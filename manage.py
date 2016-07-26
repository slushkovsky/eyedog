from argparse import ArgumentParser
from utils import args_types

from app import App

def parse_args(): 
	parser = ArgumentParser(description='Visual recognition system')
	modes  = parser.add_subparsers(dest='mode') 

	run_parser = modes.add_parser('run')
	run_parser.add_argument('--capture',   type=args_types.capture, help='Number for select a camera and path to select a video')
	run_parser.add_argument('--resize-to', type=args_types.size,    help='All frames will be resized to this size')
	run_parser.set_defaults(capture=0, resize_to='1080x720')

	return parser.parse_args()



if __name__ == '__main__': 
	args = parse_args()

	if mode == 'run': 
		app = App(args.capture, args.resize_to)
		app.run()

