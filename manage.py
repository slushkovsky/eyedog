from argparse import ArgumentParser
from utils import args_types

from app import App

def parse_args(): 
	parser = ArgumentParser(description='Visual recognition system')
	modes  = parser.add_subparsers(dest='mode') 

	run_parser = modes.add_parser('run')
	run_parser.add_argument('--input',     type=args_types.capture,      help='Number for select a camera and path to select a video')
	run_parser.add_argument('--resize-to', type=args_types.size,         help='All frames will be resized to this size')
	run_parser.add_argument('--output',    type=args_types.video_output, help='Video file path of \'screen\'')
	run_parser.set_defaults(input=0, resize_to='1080x720', output='screen')

	return parser.parse_args()



if __name__ == '__main__': 
	args = parse_args()

	if args.mode == 'run': 
		app = App(args.input, args.resize_to, args.output)
		app.run()
	else: 
		print('Unknown mode: {!r}'.format(args.mode))

