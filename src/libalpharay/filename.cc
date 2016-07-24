/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "filename.h"


void FileName::set(std::string filename) { 
	relavitePath_ = filename;
	fixRelativePath();
}


void FileName::setRootPath(std::string path){ 
	rootPath_ = path;
	fixRelativePath();
}


void FileName::fixRelativePath() {
	if (rootPath_ != "") {
		if (relavitePath_[0] == '/') {
			std::size_t found = relavitePath_.find(rootPath_);
			if (found != std::string::npos) {
				if (found == 0) {
					relavitePath_.erase(0, rootPath_.size());
					std::cout << relavitePath_ << std::endl;
					if (relavitePath_[0] == '/') {
						relavitePath_.erase(0, 1);
					}
				}
			}
		}
	}
}


std::string FileName::relPath() {
	return relavitePath_;
}


std::string FileName::absPath() {
	if (relavitePath_ == "") 
		return "";

	if (relavitePath_[0] == '/')
		return relavitePath_;

	if (rootPath_ == "")
		return relavitePath_;

	return rootPath_ + "/" + relavitePath_;	
}
