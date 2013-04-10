/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * raytracer-a
 * Copyright (C) Ali Aafee 2011 <ali.aafee@gmail.com>
 * 
 * raytracer-a is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * raytracer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "light.h"

class SpotLight: public Light 
{
public:
    Vector target_;
    float angle_;
    float angleFalloff_;

    virtual void init() { xmlName="spotlight"; }

    SpotLight() {init();};
	SpotLight(std::string name, Vector position, Vector target, float angle, Vector intensity) 
        : Light(name, position, intensity) 
		{ init(); target_ = target; angle_ = angle; angleFalloff_ = angle_;}

    Vector getIntensityByAngle(Vector intensity, Vector PO, Vector TO);

    virtual Vector getIntensity(std::vector<Object*>* objects, Vector &point, Object *ignore);
    
    virtual bool loadXml(TiXmlElement* pElem);
    virtual TiXmlElement* getXml();
};

#endif // _SPOT_LIGHT_H_

