#include <string> 
#include <vector> 
#include <algorithm>
#include <memory> 
#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define CPU_ONLY
#include <caffe/caffe.hpp>

using namespace caffe; // !! TODO
using std::string;
using std::vector;
using cv::Mat;


class Classifier {
public:
    Classifier(const string& model_file, const string& trained_file, const string& mean_file);

    vector<float> predict(const Mat& img);

private:
    void set_mean(const string& mean_file);

    void wrap_input_layer(vector<Mat>* input_channels);
    void preprocess(const Mat& img, vector<Mat>* input_channels);

private:
    std::shared_ptr<Net<float>> net_;

    cv::Size input_geometry_;
    int num_channels_;
    Mat mean_;
    bool with_mean_;
};

Classifier::Classifier(const string& model_file, const string& trained_file, 
                       const string& mean_file) {
    Caffe::set_mode(Caffe::CPU);

    /* Load the network. */
    this->net_.reset(new Net<float>(model_file, TEST));
    this->net_->CopyTrainedLayersFrom(trained_file);

    CHECK_EQ(this->net_->num_inputs(),  1) << "Network should have exactly one input.";
    CHECK_EQ(this->net_->num_outputs(), 1) << "Network should have exactly one output.";

    Blob<float>* input_layer = this->net_->input_blobs()[0];
    num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1) << "Input layer should have 1 or 3 channels.";
    this->input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

    /* Load the binaryproto mean file. */
    if (!mean_file.empty()) {
        this->set_mean(mean_file);
        this->with_mean_ = true; 
    }
    else 
        this->with_mean_ = false;
}

/* Return the indices of the top N values of vector v. */
static vector<int> argmax(const std::vector<float>& v, int N) {
    vector<std::pair<float, int>> pairs;
    
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(std::make_pair(v[i], i));

    std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), 
        [](const std::pair<float, int>& lhs, const std::pair<float, int>& rhs) {
            return lhs.first > rhs.first;
    });

    vector<int> result;
    
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    
    return result;
}

/* Load the mean file in binaryproto format. */
void Classifier::set_mean(const string& mean_file) {
    BlobProto blob_proto;
    ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

    /* Convert from BlobProto to Blob<float> */
    Blob<float> mean_blob;
    mean_blob.FromProto(blob_proto);
    
    CHECK_EQ(mean_blob.channels(), this->num_channels_)
    << "Number of channels of mean file doesn't match input layer.";

    /* The format of the mean file is planar 32-bit float BGR or grayscale. */
    vector<Mat> channels;
    float* data = mean_blob.mutable_cpu_data();

    for (int i = 0; i < this->num_channels_; ++i) {
        /* Extract an individual channel. */
        Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        channels.push_back(channel);
        data += mean_blob.height() * mean_blob.width();
    }

    /* Merge the separate channels into a single image. */
    Mat mean;
    cv::merge(channels, mean);

    /* Compute the global mean pixel value and create a mean image
     * filled with this value. */
    cv::Scalar channel_mean = cv::mean(mean);
    this->mean_ = Mat(this->input_geometry_, mean.type(), channel_mean);
}

vector<float> Classifier::predict(const Mat& img) {
    Blob<float>* input_layer = this->net_->input_blobs()[0];
    input_layer->Reshape(1, this->num_channels_, this->input_geometry_.height, 
        this->input_geometry_.width);
    /* Forward dimension change to all layers. */
    this->net_->Reshape();

    vector<Mat> input_channels;
    this->wrap_input_layer(&input_channels);

    this->preprocess(img, &input_channels);

    this->net_->Forward();

    /* Copy the output layer to a std::vector */
    Blob<float>* output_layer = this->net_->output_blobs()[0];
    const float* begin = output_layer->cpu_data();
    const float* end = begin + output_layer->channels();

    return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void Classifier::wrap_input_layer(vector<Mat>* input_channels) {
    Blob<float>* input_layer = this->net_->input_blobs()[0];

    int width = input_layer->width();
    int height = input_layer->height();

    float* input_data = input_layer->mutable_cpu_data();
    
    for (int i = 0; i < input_layer->channels(); ++i) {
        Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }
}

void Classifier::preprocess(const Mat& img, vector<Mat>* input_channels) {
    /* Convert the input image to the input image format of the network. */
    Mat sample;
    int contevt_code = (img.channels() == 3 && num_channels_ == 1) ? cv::COLOR_BGR2GRAY  :
                       (img.channels() == 4 && num_channels_ == 1) ? cv::COLOR_BGRA2GRAY :
                       (img.channels() == 4 && num_channels_ == 3) ? cv::COLOR_BGRA2BGR  :
                       (img.channels() == 1 && num_channels_ == 3) ? cv::COLOR_GRAY2BGR  : -1;
        
    if (contevt_code == -1) 
        sample = img;
    else
        cv::cvtColor(img, sample, contevt_code);

    cv::Mat sample_resized;
    
    if (sample.size() != this->input_geometry_)
        cv::resize(sample, sample_resized, this->input_geometry_);
    else
        sample_resized = sample;

    cv::Mat sample_float;
    sample_resized.convertTo(sample_float, this->num_channels_ == 3 ? CV_32FC3 : CV_32FC1);

    cv::Mat sample_normalized;

    if (this->with_mean_)
        cv::subtract(sample_float, mean_, sample_normalized);
    else 
        sample_normalized = sample_float;

    /* This operation will write the separate BGR planes directly to the
     * input layer of the network because it is wrapped by the cv::Mat
     * objects in input_channels. */
    cv::split(sample_normalized, *input_channels);

    CHECK(reinterpret_cast<float*>(input_channels->at(0).data) 
        == this->net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}