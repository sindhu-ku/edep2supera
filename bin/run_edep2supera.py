#!/usr/bin/python3
from edep2supera import utils, config
import sys,os

from optparse import OptionParser
...
parser = OptionParser(usage='Provide option flags followed by a list of input files.')
parser.add_option("-o", "--output", dest="output_filename", metavar="FILE",
	help="output (LArCV) filename")
parser.add_option("-c", "--config", dest="config",
	help="configuration (a filename or pre-defined key)")
parser.add_option("-n", "--num_events", dest="num_events", metavar="INT", default=-1,
	help="number of events to process")
parser.add_option("-s", "--skip", dest="skip", metavar="INT", default=0,
	help="number of first events to skip")

(data, args) = parser.parse_args()

if os.path.isfile(data.output_filename):
	print('Ouput file already exists:',data.output_filename)
	print('Exiting')
	sys.exit(1)

if not data.config in config.list_config() and not os.path.isfile(data.config):
	print('Invalid configuration given:',data.config)
	print('The argument is not valid as a file path nor matched with any of')
	print('predefined config keys:',config.list_config())
	print('Exiting')
	sys.exit(2)




if len(args) < 1:
	print('No input files given! Exiting')
	sys.exit(3)

output = sys.argv[1]
input_files = sys.argv[2:]

if os.path.isfile(sys.argv[1]):
	print('Output file already exists:',output)
	sys.exit(2)

utils.run_supera(out_file=data.output_filename,
	in_files=args,
	config_key=data.config,
	num_events=int(data.num_events),
	num_skip=int(data.skip)
	)
