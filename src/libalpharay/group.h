/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>
#include <tinyxml.h>
#include <string>

#include "vector.h"

#include "light.h"
#include "spot-light.h"
#include "area-light.h"
#include "sky-light.h"
#include "sun-light.h"

#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "cone.h"
#include "mesh.h"


class Group : virtual public Object, virtual public XmlObjectNamed
{
	public:		
		void init();

		Group(std::string name, BaseObject* parent=NULL) 
			: XmlObjectNamed ("group", name) 
			{ init(); parent_ = parent; }

		~Group() {};
		
		virtual void transform();

        virtual BaseObject* intersection(Ray &ray, float *t, float limit);

		Light* add(Light *light);
		Light* getLight(std::string name)
			{return getByName <Light> (name, lights);}

		Object* add(Object *object);	
		Object* getObject(std::string name)
			{return getByName <Object> (name, objects);}

		//virtual TiXmlElement* getXml();
		//virtual bool loadXml(TiXmlElement* pElem, std::string path);

		virtual TiXmlElement* getXml();
		virtual bool loadXml(TiXmlElement* pElem, std::string path);

		virtual std::vector<BaseEditableLink*>* getEditableLinksList() {
			return &combinedEditableLinks_;
		}

	protected:
		std::vector<Light*> lights;
		std::vector<Object*> objects;

	private:
		std::vector<BaseEditableLink*> combinedEditableLinks_;
		
		template <typename T> T* getByName(std::string name, std::vector<T*> &list) {
			for (unsigned long i = 0; i < list.size(); i++) {
				if (list[i]->name() == name) {
					return list[i];
				}
			}
			return NULL;
		}

		bool fromXml(TiXmlElement* pElem, Light** light, std::string path);
		bool fromXml(TiXmlElement* pElem, Object** object, std::string path);

		template< typename T > void addFromXml(TiXmlElement* pElem, std::string path);

};

#endif // _GROUP_H_
