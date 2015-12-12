#pragma once
#include "../file_io.h"
#include "shp_types.h"


namespace abeosys{
	namespace io{
		using namespace shp;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	A shp file. </summary>
	/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
	///-------------------------------------------------------------------------------------------------	
	class shp_file :public file_io
	{
	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
		///-------------------------------------------------------------------------------------------------

		shp_file(){
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Destructor. </summary>
		/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
		///-------------------------------------------------------------------------------------------------

		~shp_file(){
			dispose_data();
		}
		void dispose_data(){
			while (_records.size() > 0){
				shp::record* rec = _records.back();
				_records.pop_back();
				if (rec) delete rec;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads the given SHP file.
		/// 			Check the details of file format in this link: https://www.esri.com/library/whitepapers/pdfs/shapefile.pdf 			
		/// 			</summary>
		/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
		/// <param name="path">	The path to load. </param>
		///-------------------------------------------------------------------------------------------------

		void load(const string& path){
			file_io::load(path); 

			//load this file as binary
			_file.open(path, ios::in|ios::binary); 
			//reading header
			//byte 0 - file code 4 bytes big endian	
			_file.seekg(0);

			//read file header
			_header.read(_file);
/*#ifdef _DEBUG
			cout << "FILE_CODE      = " << _header.file_code << endl;
			cout << "FILE_LENGTH    = " << _header.length << " WORDS"<<endl;
			cout << "FILE_VERSION   = " << _header.version << endl;
			cout << "SHAPE_TYPE     = " << _header.shape_type << endl;
			cout << "XMIN           = " << _header.xmin << endl;
			cout << "XMAX           = " << _header.xmax << endl;
			cout << "YMIN           = " << _header.ymin << endl;
			cout << "YMAX           = " << _header.ymax << endl;
			cout << "ZMIN           = " << _header.zmin << endl;
			cout << "ZMAX           = " << _header.zmax << endl;
			cout << "MMIN           = " << _header.mmin << endl;
			cout << "MMAX           = " << _header.mmax << endl;
#endif		*/	

			size_t word_count = 50;

			dispose_data();
			int record_count = 0;
			while(word_count < _header.length){
				//read first record
				shp::record *rec = new shp::record();
				rec->read(_file);
				//if (record.shape_type != polygon_shape){
				//	//stop here
				//	record.shape_type = 0;
				//}
				_records.push_back(rec);
				word_count += rec->length + 4;
				record_count++;
				//printf("-------------------------------\n");
				//printf("record num  = %d\n", record.number);
				//printf("record len  = %d\n", record.length);
				//printf("record type = %d\n",record.shape_type);				
				//printf("part count  = %d\n", ((polygon*)record.entity)->part_count);
				//printf("point count = %d\n", ((polygon*)record.entity)->point_count);
			}
			//cout << "RECORD COUNT = " << record_count << endl;

			_file.close();
			//_file.open("G:\\data1.txt", ios::out);
			//_file << _header.xmin << ","
			//	<< _header.xmax << ","
			//	<< _header.ymin << ","
			//	<< _header.ymax << endl;
			//for (int i = 0; i < _records.size(); i++){
			//	polygon *entity =(polygon*)_records[i]->entity;
			//	_file << entity->point_count << ";";
			//	for (int k = 0; k < entity->point_count; k++){
			//		point& p = entity->points[k];
			//		_file << p.x << "," << p.y << ":";
			//	}
			//	_file << ";"<< _records[i]->shape_type<< endl;
			//}

			//_file.close();
			//cout << "DONE" << endl;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Saves the given file. </summary>
		/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
		/// <param name="path">	The path to save. </param>
		///-------------------------------------------------------------------------------------------------

		void save(const string& path){
			//Not implemented
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the records. </summary>
		/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
		/// <returns>	null if it fails, else a vector&lt;record*&gt;&amp; </returns>
		///-------------------------------------------------------------------------------------------------

		vector<shp::record*>& records(){ return _records; }
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Record count. </summary>
		///
		/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
		///
		/// <returns>	A size_t. </returns>
		///-------------------------------------------------------------------------------------------------

		size_t record_count(){ return _records.size(); }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Records the given identifier. </summary>
		///
		/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
		///
		/// <param name="id">	The identifier. </param>
		///
		/// <returns>	null if it fails, else a dbf::record*. </returns>
		///-------------------------------------------------------------------------------------------------

		shp::record* record(size_t id){ return _records[id]; }

		shp::header& header(){ return _header; }

	private:
		/// <summary>	The header. </summary>
		shp::header _header;
		/// <summary>	The records. </summary>
		vector<shp::record*> _records;
	};
}}

