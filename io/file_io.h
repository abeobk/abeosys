#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "../abeosyslib.h"


using namespace std;
///-------------------------------------------------------------------------------------------------
// namespace: abeosys
//
// summary:	.
///-------------------------------------------------------------------------------------------------

namespace abeosys{


///-------------------------------------------------------------------------------------------------
// namespace: io
//
// summary:	.
///-------------------------------------------------------------------------------------------------

	namespace io{

		///-------------------------------------------------------------------------------------------------
		/// <summary>	A file i/o. </summary>
		///
		/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
		///-------------------------------------------------------------------------------------------------

		class file_io{
		public:

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Loads this object. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///
			/// <param name="path">	The path to load. </param>
			///-------------------------------------------------------------------------------------------------

			virtual void load(const string& path){
				_path = path;

			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Saves the given file. </summary>
			///
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			///
			/// <param name="path">	The path to save. </param>
			///-------------------------------------------------------------------------------------------------

			virtual void save(const string& path)=0;

		protected:

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Reads value of type T in big-endianess. </summary>
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			/// <typeparam name="T">	Generic type parameter. </typeparam>
			/// <returns>	value that is converted back to small-endian </returns>
			///-------------------------------------------------------------------------------------------------

			template<typename T>
			T read_big(){
				T value;
				_file.read((char*)&value, sizeof(T));
				switch (sizeof(T)){
				case 4:	return _byteswap_ulong(value);
				case 2: return _byteswap_ushort(value);
				case 8: return _byteswap_uint64(value);
				default: return NULL;
				}
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Reads value of type T at a given position in big-endianess. </summary>
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			/// <typeparam name="T">	Generic type parameter. </typeparam>
			/// <returns>	value that is converted back to small-endian </returns>
			///-------------------------------------------------------------------------------------------------

			template<typename T>
			T read_big(const size_t pos){
				T value;
				_file.seekg(pos);
				_file.read((char*)&value, sizeof(T));
				switch (sizeof(T)){
				case 4:	return _byteswap_ulong(value);
				case 2: return _byteswap_ushort(value);
				case 8: return _byteswap_uint64(value);
				default: return NULL;
				}
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Reads value of type T in little endianess. </summary>
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			/// <typeparam name="T">	Generic type parameter. </typeparam>
			/// <returns>	The value </returns>
			///-------------------------------------------------------------------------------------------------

			template<typename T>
			T read_little(){
				T value;
				_file.read((char*)&value, sizeof(T));
				return value;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Reads value of type T at a given position in little endianess. </summary>
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			/// <typeparam name="T">	Generic type parameter. </typeparam>
			/// <returns>	The value</returns>
			///-------------------------------------------------------------------------------------------------

			template<typename T>
			T read_little(const size_t pos){
				T value;
				_file.seekg(pos);
				_file.read((char*)&value, sizeof(T));
				return value;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Reads a structure. </summary>
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			/// <typeparam name="T">	Generic type parameter. </typeparam>
			/// <param name="ptr">	[in,out] If non-null, the pointer. </param>
			/// <param name="pos">	The position. </param>
			///-------------------------------------------------------------------------------------------------

			template<typename T>
			void read_struct(const T* ptr,const size_t pos){
				_file.seekg(pos);				
				_file.read((char*)ptr, sizeof(T));
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Reads a structure. </summary>
			/// <remarks>	Do Van Phu, 12/5/2015. </remarks>
			/// <typeparam name="T">	Generic type parameter. </typeparam>
			/// <param name="ptr">	The pointer. </param>
			///-------------------------------------------------------------------------------------------------

			template<typename T>
			void read_struct(const T* ptr){
				_file.read((char*)ptr, sizeof(T)); 
			}
			

			template<typename T>
			void swap_endianess(T* value){
				switch (sizeof(T)){
				case 4:	*value = _byteswap_ulong(*value); break;
				case 2: *value = _byteswap_ushort(*value); break;
				case 8: *value = _byteswap_uint64(*value); break;
				}
			}
		protected:
			/// <summary>	Full pathname of the file. </summary>
			string _path;
			/// <summary>	The file. </summary>
			fstream _file;
		};
	}
}