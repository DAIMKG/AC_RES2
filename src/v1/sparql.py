import urllib
import urllib.request
import argparse
import sys

parser = argparse.ArgumentParser(description = 'A simple sparql executor. The output is ./result.txt')

parser.add_argument('-url', help = 'executor url; default http://dbpedia.org/sparql', default = 'http://dbpedia.org/sparql')
parser.add_argument('-namespace', dest = 'ns', help = 'default-graph-url; default http://dbpedia.org', default = 'http://dbpedia.org')
parser.add_argument('-format', help = 'result format(json or html); default json', default = 'json')
parser.add_argument('-timeout', help = 'set the time limit (>=1000)ms of executing; default 30000(ms)', type = int, default = 30000)
parser.add_argument('SPARQL_FILE', help = 'indicate the sparql file')

args = parser.parse_args()

url = args.url
data = {}
data['default-graph-url'] = args.ns

f = open(args.SPARQL_FILE, 'r')
data['query'] = f.read();

if args.format == 'json':
	data['format'] = 'application/sparql-results+json'
elif args.format == 'html':
	data['format'] = 'text/html'

data['timeout'] = args.timeout
data['CXML_redir_for_subjs'] = '121'
data['debug'] = 'on'

data = urllib.parse.urlencode(data)

out = open('result.txt', 'w');
out.write(bytes.decode(urllib.request.urlopen(url + '?%s' % data).read()))
