#pragma once
#include <vector>
#include <fstream>
#include <memory>

#pragma pack(push,4)

using namespace std;
namespace abeosys{
	namespace io{
		namespace shp{

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Defines an alias representing the enum. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			typedef enum{
				null_shape = 0,
				point_shape = 1,
				poly_line_shape = 3,
				polygon_shape = 5,
				multi_point_shape = 8,
				point_z_shape = 11,
				poly_line_z_shape = 13,
				polygon_z_shape = 15,
				multi_point_z_shape = 18,
				point_m_shape = 21,
				poly_line_m_shape = 23,
				polygon_m_shape = 25,
				multi_point_m_shape = 28,
				multi_patch_shape = 31
			}shape_type;

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A header. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class shp_entity{
			public:
				virtual void read(fstream& file) {};
				template<typename T>
				static inline void swap_endianess(T* value){
					switch (sizeof(T)){
					case 4:	*value = _byteswap_ulong(*value); break;
					case 2: *value = _byteswap_ushort(*value); break;
					case 8: *value = _byteswap_uint64(*value); break;
					}
				}
				template<typename T>
				static inline T read_big(fstream& file){
					T value;
					file.read((char*)&value, sizeof(T));
					switch (sizeof(T)){
					case 4:	return _byteswap_ulong(value);
					case 2: return _byteswap_ushort(value);
					case 8: return _byteswap_uint64(value);
					default: return NULL;
					}
				}
				template<typename T>
				static inline T read_little(fstream& file){
					T value;
					file.read((char*)&value, sizeof(T));
					return value;
				}
			};

			class header: public shp_entity{
			public:
				uint32_t file_code;
				uint32_t unused[5];
				uint32_t length;
				uint32_t version;
				uint32_t shape_type;
				double xmin;
				double ymin;
				double xmax;
				double ymax;
				double zmin;
				double zmax;
				double mmin;
				double mmax;
				void read(fstream& file){
					file_code = read_big<int>(file);
					file.read((char*)unused, 5 * sizeof(int));
					length = read_big<int>(file);
					version = read_little<int>(file);
					shape_type = read_little<int>(file);
					xmin = read_little<double>(file);
					ymin = read_little<double>(file);
					xmax = read_little<double>(file);
					ymax = read_little<double>(file);
					zmin = read_little<double>(file);
					zmax = read_little<double>(file);
					mmin = read_little<double>(file);
					mmax = read_little<double>(file);
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	An entity. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class entity: public shp_entity{
			};


			///-------------------------------------------------------------------------------------------------
			/// <summary>	A point. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class point:public entity{
			public:
				union{
					double vals[2];
					struct{
						double x;
						double y;
					};
				};
				void read(fstream& file){
					file.read((char*)vals, 2 * sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A pouint32_t m. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class point_m :public entity{
			public:
				union{
					double vals[3];
					struct{
						double x;
						double y;
						double m;
					};
				};
				void read(fstream& file){
					file.read((char*)vals, 3 * sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A pouint32_t z coordinate. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class point_z : public point_m{
			public:				
				union{
					double vals[4];
					struct{
						double x;
						double y;
						double z;
						double m;
					};
				};
				void read(fstream& file){
					file.read((char*)vals, 4 * sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A multi point. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class multi_point:public entity{
			public:
				double bound[4]; //bounding box
				uint32_t point_count;//number of points
				vector<point> points; //point
				void read(fstream& file){
					file.read((char*)bound, 4 * sizeof(double));
					point_count = read_little<int>(file);
					points.resize(point_count);
					for (uint32_t i = 0; i < point_count; i++)
						points[i].read(file);
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A multi pouint32_t m. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class multi_point_m :public multi_point{
			public:
				double m_range[2];
				vector<double> m_arrays;
				void read(fstream& file){
					multi_point::read(file);
					file.read((char*)m_range, 2 * sizeof(double));
					m_arrays.resize(point_count);
					file.read((char*)&m_arrays.front(), point_count*sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A multi pouint32_t z coordinate. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class multi_point_z :public multi_point_m{
			public:
				double z_range[2];
				vector<double> z_arrays;
				void read(fstream& file){
					multi_point::read(file);
					file.read((char*)z_range, 2 * sizeof(double));
					z_arrays.resize(point_count);
					file.read((char*)&z_arrays.front(), point_count*sizeof(double));
					file.read((char*)m_range, 2 * sizeof(double));
					m_arrays.resize(point_count);
					file.read((char*)&m_arrays.front(), point_count*sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A polygon line. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class poly_line:public multi_point{
			public:
				uint32_t part_count;//number of parts
				vector<int> parts;//index to first pouint32_t in parts
				void read(fstream& file){
					file.read((char*)bound, 4 * sizeof(double));
					part_count = read_little<int>(file);
					point_count = read_little<int>(file);
					parts.resize(part_count);
					//read all parts
					for (uint32_t i = 0; i < part_count; i++)
						parts[i] = read_little<int>(file);
					points.resize(point_count);
					for (uint32_t i = 0; i < point_count; i++)
						points[i].read(file);					
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A polygon line m. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class poly_line_m :public poly_line{
			public:
				double m_range[2];
				vector<double> m_arrays;
				void read(fstream& file){
					poly_line::read(file);
					file.read((char*)m_range, 2 * sizeof(double));
					m_arrays.resize(point_count);
					file.read((char*)&m_arrays.front(), point_count*sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A polygon line z coordinate. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class poly_line_z :public poly_line_m{
			public:
				double z_range[2];
				vector<double> z_arrays;
				void read(fstream& file){
					poly_line::read(file);
					file.read((char*)z_range, 2 * sizeof(double));
					z_arrays.resize(point_count);
					file.read((char*)&z_arrays.front(), point_count*sizeof(double));
					file.read((char*)m_range, 2 * sizeof(double));
					m_arrays.resize(point_count);
					file.read((char*)&m_arrays.front(), point_count*sizeof(double));
				}
			};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A polygon. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class polygon :public poly_line{};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A polygon m. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class polygon_m : public poly_line_m{};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A polygon z coordinate. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class polygon_z : public poly_line_z{};

			///-------------------------------------------------------------------------------------------------
			/// <summary>	A multi patch. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class multi_patch: public polygon_z{
				vector<int> part_types;
				void read(fstream& file){
					file.read((char*)bound, 4 * sizeof(double));
					part_count = read_little<int>(file);
					point_count = read_little<int>(file);
					parts.resize(part_count);
					for (uint32_t i = 0; i < part_count; i++)
						parts[i] = read_little<int>(file);
					part_types.resize(part_count);
					for (uint32_t i = 0; i < part_count; i++)
						part_types[i] = read_little<int>(file);

					points.resize(point_count);
					for (uint32_t i = 0; i < point_count; i++)
						points[i].read(file);

					file.read((char*)z_range, 2 * sizeof(double));
					z_arrays.resize(point_count);
					file.read((char*)&z_arrays.front(), point_count*sizeof(double));
					file.read((char*)m_range, 2 * sizeof(double));
					m_arrays.resize(point_count);
					file.read((char*)&m_arrays.front(), point_count*sizeof(double));
				}
			};			


			///-------------------------------------------------------------------------------------------------
			/// <summary>	A record. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///-------------------------------------------------------------------------------------------------

			class record :public shp_entity{
			public:
				record(){
					entity = NULL;
				}
				~record(){
					if (entity)delete entity;
				}
				uint32_t number;
				uint32_t length;
				uint32_t shape_type;
				entity* entity;			
				void read(fstream& file){
					number = read_big<int>(file);
					length = read_big<int>(file);
					shape_type = read_little<int>(file);
					switch (shape_type){
					case	null_shape:break;
					case 	point_shape:entity = new point(); entity->read(file); break;
					case 	point_z_shape:entity = new point_z(); entity->read(file); break;
					case 	point_m_shape:entity = new point_m(); entity->read(file); break;

					case 	poly_line_shape:entity = new poly_line(); entity->read(file); break;
					case 	poly_line_z_shape:entity = new poly_line_z(); entity->read(file); break;
					case 	poly_line_m_shape:entity = new poly_line_m(); entity->read(file); break;

					case 	polygon_shape: entity = new polygon(); entity->read(file); break;
					case 	polygon_z_shape: entity = new polygon_z(); entity->read(file); break;
					case 	polygon_m_shape: entity = new polygon_m(); entity->read(file); break;

					case 	multi_point_shape:entity = new multi_point(); entity->read(file); break;
					case 	multi_point_z_shape:entity = new multi_point_z(); entity->read(file); break;
					case 	multi_point_m_shape:entity = new multi_point_m(); entity->read(file); break;

					case 	multi_patch_shape:entity = new multi_patch(); entity->read(file); break;
					}
				}
			};
		}
	}
}
#pragma pack(pop)