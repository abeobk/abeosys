#pragma once
#include "../file_io.h"
#include "dbf_types.h"


using namespace std;
namespace abeosys{
	namespace io{

		///-------------------------------------------------------------------------------------------------
		/// <summary>	A dbf file. </summary>
		///
		/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
		///-------------------------------------------------------------------------------------------------

		class dbf_file :public file_io{
		public:

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Default constructor. </summary>
			///
			/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			dbf_file(){}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Destructor. </summary>
			///
			/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			~dbf_file(){
				dispose_data();
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Dispose data. </summary>
			///
			/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			void dispose_data(){
				while (_records.size() > 0){
					dbf::record* rec = _records.back();
					_records.pop_back();
					if (rec) delete rec;
				}
				while (_fields.size() > 0){
					dbf::field* field = _fields.back();
					_fields.pop_back();
					if (field) delete field;
				}
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Loads the given file. </summary>
			///
			/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
			///
			/// <param name="path">	The path to load. </param>
			///-------------------------------------------------------------------------------------------------

			void load(const string& path){
				file_io::load(path);
				//load this file as binary
				_file.open(path, ios::in | ios::binary);
				//reading header
				//byte 0 - file code 4 bytes big endian	
				_file.seekg(0);

				_header.read(_file);
				int header_count = 32;
				dispose_data();

				int byte_index = 0;
				while (header_count < _header.header_length-2){
					dbf::field* field = new dbf::field();
					field->read(_file);
					field->index = byte_index;
					byte_index += field->length;
					_fields.push_back(field);
					header_count += 32;
					//check if catch od
				}
				char buff[2];
				//check 0D 20
				_file.read(buff, 2);
				_ASSERTE(buff[0] == 0x0D && buff[1] == 0x20);

				//read all record
				_records.resize(_header.record_count);
				for (int i = 0; i < _header.record_count; i++){
					dbf::record *record = new dbf::record();
					record->data.resize(_header.record_length);
					record->read(_file);
					_records[i] = record;
				}	
				_file.close();
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Saves the given file. </summary>
			///
			/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
			///
			/// <param name="path">	The path to save. </param>
			///-------------------------------------------------------------------------------------------------

			void save(const string& path){

			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Gets the records. </summary>
			///
			/// <remarks>	Do Van Phu, 12/6/2015. </remarks>
			///
			/// <returns>	null if it fails, else a vector&lt;dbf::record*&gt;&amp; </returns>
			///-------------------------------------------------------------------------------------------------

			vector<dbf::record*>& records(){ return _records; }

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

			dbf::record* record(size_t id){ return _records[id]; }

			dbf::field* field(int id){ return _fields[id]; }
			double get_double(int record_id, int field_id){
				return _records[record_id]->get_double(_fields[field_id]);
			}
			string get_string(int record_id, int field_id){
				return _records[record_id]->get_string(_fields[field_id]);
			}
			int32_t get_int32(int record_id, int field_id){
				return _records[record_id]->get_int32(_fields[field_id]);
			}
			int64_t get_int64(int record_id, int field_id){
				return _records[record_id]->get_int64(_fields[field_id]);
			}

		private:
			/// <summary>	The header. </summary>
			dbf::header _header;
			/// <summary>	The fields. </summary>
			vector<dbf::field*> _fields;
			/// <summary>	The records. </summary>
			vector<dbf::record*> _records;
		};
	}
}

