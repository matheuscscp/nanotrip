#include <fstream>
#include <sstream>

#include "common.hpp"

#include "Ranking.hpp"

#include "SDLBase.hpp"

using namespace common;

using std::string;
using std::fstream;
using std::stringstream;

// ==========================================================================
// Private
// ==========================================================================

struct RecordData
{
	char name[21];
	unsigned int points;
	char date[25];
	
	string str(unsigned int max_points);
};

class DataManager
{
private:
	string filename;
	unsigned int max_slots;
	RecordData* array;
public:
	DataManager(const string& filename, unsigned int max_slots = 0);
	~DataManager();
	
	unsigned int maxSlots() const;
	
	bool setText(Text* text, unsigned int i, unsigned int max_points) const;
	
	static unsigned int save(
		const RankingData& newdata,
		const string& filename,
		unsigned int max_slots = 0
	);
private:
	unsigned int insert(const RankingData& newdata);
};

string RecordData::str(unsigned int max_points)
{
	// buffer
	int tmp = 0;
	char ret[63];
	for( int i = 0; i < 62; ++i )
		ret[i] = ' ';
	ret[62] = '\0';
	
	// name
	for( int i = 0; ( i < 20 ) && ( name[i] != '\0' ); ++i )
	{
		ret[tmp] = name[i];
		++tmp;
	}
	
	// points
	{
		stringstream ss1, ss2;
		ss1 << max_points;
		ss2 << "%0";
		ss2 << ss1.str().size();
		ss2 << "d";
		
		char stmp[11];
		sprintf( stmp, ss2.str().c_str(), points );
		
		tmp = 24 + ( 10 - ss1.str().size() );
		
		for( int i = 0; i < int( ss1.str().size() ); ++i )
		{
			ret[tmp] = stmp[i];
			++tmp;
		}
	}
	
	// date
	tmp = 38;
	for( int i = 0; i < 24; ++i )
	{
		ret[tmp] = date[i];
		++tmp;
	}
	
	return string( ret );
}

DataManager::DataManager(const string& filename, unsigned int max_slots) :
filename(filename), max_slots(max_slots)
{
	fstream f( RootPath::get( filename ).c_str(), fstream::binary | fstream::in );
	
	// if max_slots is not set, then ranking will loaded and rendered
	if( !this->max_slots )
	{
		if( !f.is_open() )
			throw( mexception( "Could not open ranking file" ) );
		
		f.read( (char*) &this->max_slots, sizeof( this->max_slots ) );
	}
	
	// allocating memory and initializing
	array = new RecordData[ this->max_slots ];
	for( unsigned int i = 0; i < this->max_slots; ++i )
		array[i].points = 0;
	
	// reading file
	if( f.is_open() )
	{
		for( unsigned int i = 0; i < this->max_slots; ++i )
			f.read( (char*) &array[i], sizeof( array[i] ) );
		
		f.close();
	}
}

DataManager::~DataManager()
{
	delete[] array;
}

unsigned int DataManager::maxSlots() const
{
	return max_slots;
}

bool DataManager::setText(Text* text, unsigned int i, unsigned int max_points) const
{
	if( array[i].points )
	{
		text->setText( array[i].str( max_points ) );
		return true;
	}
	
	return false;
}

unsigned int DataManager::save(
	const RankingData& newdata,
	const string& filename,
	unsigned int max_slots
)
{
	DataManager data( filename, max_slots );
	unsigned int ret = data.insert( newdata );
	
	if( ret != max_slots )
	{
		fstream f( RootPath::get( filename ).c_str(), fstream::binary | fstream::out );
		
		f.write( (const char*) &max_slots, sizeof( max_slots ) );
		for( unsigned int i = 0; i < max_slots; ++i )
			f.write( (const char*) &data.array[i], sizeof( data.array[i] ) );
		
		f.close();
	}
	
	return ret;
}

unsigned int DataManager::insert(const RankingData& newdata)
{
	unsigned int pos = max_slots;
	
	// look for a position in the array
	for( unsigned int i = 0; ( i < max_slots ) && ( pos == max_slots ); ++i )
	{
		if( array[i].points < newdata.points )
			pos = i;
	}
	
	// pull the array and insert the new record
	if( pos != max_slots )
	{
		// pull
		for( unsigned int i = max_slots - 1; i >= pos + 1; --i )
			array[i] = array[ i - 1 ];
		
		// copying name
		int i;
		for( i = 0; ( i < int( newdata.name.size() ) ) && ( i < 21 ); ++i )
			array[pos].name[i] = ( ( i < 20 ) ? newdata.name[i] : '\0' );
		if( i < 21 )
			array[pos].name[i] = '\0';
		
		array[pos].points = newdata.points;
		
		// getting current date and time
		time_t rawtime = time( NULL );
		strcpy( array[pos].date, ctime( &rawtime ) );
	}
	
	return pos;
}

// ==========================================================================
// Interface
// ==========================================================================

RankingData::RankingData(const string& name, unsigned int points) :
name(name), points(points)
{
}

RankingData::~RankingData()
{
}

unsigned int RankingData::save(const string& filename, unsigned int max_slots) const
{
	return DataManager::save( *this, filename, max_slots );
}

Ranking::Ranking(
	const string& filename, Text* text, unsigned int max_points,
	int spacing, bool center, int left, int top, int right, int bottom
)
{
	try {
		DataManager data( filename );
		unsigned int max_slots = data.maxSlots();
		bool end = false;
		
		for( unsigned int i = 0; ( i < max_slots ) && ( !end ); ++i )
		{
			if( !data.setText( text, i, max_points ) )
				end = true;
			else
			{
				int x, y;
				
				if( center )
				{
					x = ( SDLBase::screen()->w - text->w() ) / 2;
					y = (
						(
							SDLBase::screen()->h -
							( text->h() + 2 * spacing ) * max_slots
						) / 2 +
						i * ( text->h() + 2 * spacing ) +
						spacing
					);
				}
				else
				{
					if( left )
						x = left;
					else
						x = ( SDLBase::screen()->w - text->w() - right );
					
					if( top )
						y = ( top + i * ( text->h() + 2 * spacing ) + spacing );
					else
					{
						y = (
							SDLBase::screen()->h - bottom + spacing -
							( text->h() + 2 * spacing ) * ( max_slots - i )
						);
					}
				}
				
				surface.render( text, x, y );
			}
		}
	}
	catch(mexception&) {
	}
}

Ranking::~Ranking()
{
}

void Ranking::render()
{
	surface.render();
}
