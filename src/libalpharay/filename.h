/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef FILE_NAME_H
#define FILE_NAME_H

#include <string>
#include <iostream>


class FileName {
	public:
		FileName() 
			{ relavitePath_ = ""; }
		FileName(std::string relativePath) 
			{ set(relativePath); }

		void set(std::string filename);
		void setRootPath(std::string path);
		std::string relPath();
		std::string absPath();
	private:
		std::string relavitePath_;
		std::string rootPath_;

		void fixRelativePath();
};


#endif // FILE_NAME_H
