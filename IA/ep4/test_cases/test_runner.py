import sys
import os.path

def read_config( path ) :
	config = {}
	with open( '%s/CONFIG' % path ) as f :
		for line in f :
			if not line.strip() : continue
			key , val = line.strip().split( ':' )
			key = key.strip()
			val = val.strip()
			config[ key ] = config.get( key , [] )
			config[ key ].append( val )
	for key in config :
		if type( config[ key ] ) == type( [] ) :
			if len( config[ key ] ) == 1 :
				config[ key ] = config[ key ][ 0 ]
	return config

def execute( question , codeRoot = '' ) :
	if int( question[ 1 ] ) not in range( 6 ) :
		raise Exception( 'There is no question %s' % question )
	try :
		exec( 'import %s.execute as test' % question )
		config = read_config( os.path.join( codeRoot , 'test_cases' , question ) )
		return test.run( config , codeRoot = codeRoot )
	except Exception as e :
		return ( '' , e.message )
