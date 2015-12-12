#pragma once
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#pragma pack(push,1)

using namespace std;
namespace abeosys{
	namespace io{
		namespace dbf{
			class dbf_entity{
			public:
				virtual void read(fstream& file) = 0;
			};

			class field:public dbf_entity{
			public:
				uint32_t index;
				union{
					char data[32];
					struct{
						char name[11];						//0-10
						char type;							//11
						uint32_t mem_address;				//12-15
						uint8_t length;						//16
						uint8_t decimal_count;				//17
						uint16_t reserved_for_multi_user;	//18-19
						uint8_t work_area_id;				//20
						uint16_t reserved_for_multi_user1;	//21-22
						uint8_t set_fields_flag;			//23
						char reserved[7];					//24-30
						uint8_t index_field_flag;			//31
					};
				};
				void read(fstream& file){
					file.read(data, 32);
				}
			};

			class header :public dbf_entity{
			public:
				union{
					char data[32];
					struct{
						uint8_t version_number;				//0
						uint8_t year;						//1
						uint8_t month;						//2
						uint8_t day;						//3
						uint32_t record_count;				//4-7
						uint16_t header_length;				//8-9
						uint16_t record_length;				//10-11
						uint16_t reserved;					//12-13
						uint8_t incomplete_transac;			//14
						uint8_t encryption_flag;			//15
						uint32_t free_record_thread;		//16-19
						uint64_t reserved_for_multi_user;	//20-27
						uint8_t mdx_flag;					//28
						uint8_t language_driver;			//29
						uint16_t reserved1;					//30-31
					};
				};
				void read(fstream& file){
					file.read(data, 32);
				}
			};

			class record :public dbf_entity{
			public:
				string data;
				void read(fstream& file){
					file.read(&data.front(), data.size());
				}
				string inline get_string(const dbf::field* field){
					return data.substr(field->index, field->length);
				}
				int32_t inline get_int32(const dbf::field* field){
					return atoi(data.substr(field->index, field->length).c_str());
				}
				int64_t inline get_int64(const dbf::field* field){
					return atoll (data.substr(field->index, field->length).c_str());
				}
				double inline get_double(const dbf::field* field){
					return atof(data.substr(field->index, field->length).c_str());
				}
			};
		}
	}
}
#pragma pack(pop)