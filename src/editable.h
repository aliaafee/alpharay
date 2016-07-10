/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _EDITABLE_H_
#define _EDITABLE_H_

#include <string>
#include <sstream>

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

		virtual void reset() {
			*value_ = defaultValue_;
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
	//std::stringstream ss (std::stringstream::in | std::stringstream::out);
	//ss << value;
	if (value) {
		return "true";
	}
	return "false";
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

	private:
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
		}

		void setSource(std::string source) {
			source_ = source;
		}

		std::string str() {
			if (*target_ == NULL) {
				return "";
			} else {
				return (*target_)->name();
			}
		}

		std::string name() {
			return name_;
		}

		T** target() {
			return target_;
		}

	//private:
		std::string name_;
		std::string source_;
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
