/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _EDITABLE_H_
#define _EDITABLE_H_

#include <string>
#include <sstream>
#include <vector>

#include "vector.h"

class BaseEditable
{
	public:
		BaseEditable(std::string name) {
			name_ = name;
		};
		
		BaseEditable(std::string name, std::string *value, std::string defaultValue) {
			name_ = name;

			value_ = value; 
			defaultValue_ = defaultValue;
			
			noMin_ = true;
			noMax_ = true;
		}

		~BaseEditable() {};

		virtual bool set(std::string &value) {
			if (!isValid(value)) return false;
			*value_ = value; return true; 
		}

		virtual std::string str() { 
			return *value_; 
		}

		virtual bool isValid(std::string &value) { 
			return true; 
		}

		virtual std::string getMax() {
			return std::string("");
		}

		virtual std::string getMin() {
			return std::string("");
		}

		virtual std::string type() {
			return "string";
		}

		bool noMax() { return noMax_; }
		bool noMin() { return noMin_; }
		std::string name() { return name_; }
		virtual void reset() { *value_ = defaultValue_; }
	private:
		std::string name_;
		std::string *value_;
		std::string defaultValue_;
		bool noMax_;
		bool noMin_;
};


template<typename T> class Editable : public BaseEditable
{
	public:
		Editable(std::string name, T *value, T defaultValue) : BaseEditable(name) {
			value_ = value; 
			defaultValue_ = defaultValue;
			
			noMin_ = true;
			noMax_ = true;

			reset();
		}

		Editable(std::string name, T *value, T defaultValue, T max, T min) : BaseEditable(name) {
			value_ = value;
			defaultValue_ = defaultValue;

			max_ = max;
			min_ = min;

			noMin_ = false;
			noMax_ = false;

			reset();
		}

		~Editable() {};

		virtual bool set(std::string &value) {
			T newValue = fromStr(value);
	
			if (!isValid(newValue)) return false;
			
			*value_ = newValue; 
			return true; 
		}

		virtual std::string str() { 
			//std::stringstream ss (std::stringstream::in | std::stringstream::out);
			//ss << *value_;
			return toStr(*value_);
		}
		
		virtual std::string getMax() {
			return toStr(max_);
		}

		virtual std::string getMin() {
			return toStr(min_);
		}

		virtual std::string type() {
			return "string";
		}

		virtual void reset() {
			*value_ = defaultValue_;
		}

		virtual T getValue() {
			return *value_;
		}

	private:
		T *value_;
		T defaultValue_;
		T max_;
		T min_;
		bool noMax_;
		bool noMin_;

		virtual T fromStr(std::string value) {
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << value;
			T result;
			ss >> result;
			return result;
		}

		virtual std::string toStr(T value) {
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << value;
			return ss.str();
		}

		virtual bool isValid(T &value) { 
			/*
			if (!noMax_) {
				if (value > max_) {
					return false;
				}
			}
			if (!noMin_) {
				if (value < min_) {
					return false;
				}
			}
			*/
			return true; 
		}
};


template <> inline std::string Editable<bool>::toStr(bool value) {
	if (value) {
		return "true";
	}
	return "false";
}

template <> inline bool Editable<bool>::fromStr(std::string value) {
	if (value == "true") {
		return true;
	}
	return false;
}

template <> inline std::string Editable<bool>::type() {
	return "bool";
}

template <> inline std::string Editable<std::string>::type() {
	return "string";
}

template <> inline std::string Editable<int>::type() {
	return "int";
}

template <> inline std::string Editable<float>::type() {
	return "float";
}

template <> inline std::string Editable<Vector>::type() {
	return "vector";
}

template <> inline std::string Editable<Color>::type() {
	return "color";
}

template <> inline std::string Editable<Vector2>::type() {
	return "vector2";
}


class BaseEditableLink
{
	friend class XmlObjectNamed;
	public:
		BaseEditableLink(std::string name) {
			name_ = name;
		}

		virtual void setSource(std::string source) {
			source_ = source;
		}

		virtual std::string str() {
			return source_;
		}

		virtual std::string name() {
			return name_;
		}

		virtual std::string type() {
			return "";
		}

	protected:
		std::string name_;
		std::string source_;
};


template <typename T>
class EditableLink : public BaseEditableLink
{
	friend class XmlObjectNamed;
	public:
		EditableLink(std::string name, T **target) : BaseEditableLink(name) {
			name_ = name;
			target_ = target;
			*target_ = NULL;
			source_ = "";
		}

		std::string str() {
			if (*target_ == NULL) {
				return "";
			} else {
				return (*target_)->name();
			}
		}

		T** target() {
			return target_;
		}

		std::string type() {
			return "";
		}

		bool updateLinkFromList(std::vector<T*>* list) {
			*target_ = NULL;
			
			if (source_ == "") {
				std::cout << "source is empty string" << std::endl;
				return true;
			}

			for (auto it = list->begin(); it != list->end(); ++it) {
				T* object = *it;
				std::cout << object->name() << "-" << source_ << std::endl;
				if (object->name() == source_) {
					*target_ = object;
					return true;
				}
			}

			return false;
		}

		

	//private:
		T **target_;
};


/*
template<typename T, typename L> class EditableLink : public BaseEditable
{
	public:
		EditableLink(std::string name, T **target) : BaseEditable(name) {
			(*target_) = (*target); 
			//defaultTarget_ = NULL;
			
			//noMin_ = true;
			//noMax_ = true;

			//reset();
		}

		~EditableLink() {};

		virtual bool set(std::string targetname, L *linkList) {

			//linkList->template add<T>(targetname, &(*target_));
			return true; 
		}

		virtual std::string str() { 
			//std::stringstream ss (std::stringstream::in | std::stringstream::out);
			//ss << *value_;
			//return (target_)->name();
			return "x";
		}
		
		virtual void reset() {
			*(*target_) = *defaultTarget_;
		}

	private:
		T *(*target_);
		T *defaultTarget_;

		bool noMax_;
		bool noMin_;
};
*/
#endif // _EDITABLE_H_
