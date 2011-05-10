/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * Configuration.h - prototypes part for Object holding the settings
 *   of the filter
 *
 * -------------------------------------------------------------------------
 * 
 * Copyright (C) 2011  Christian Aistleitner <christian@quelltextlich.at>
 *
 * -------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * -------------------------------------------------------------------------
 */
 
#ifndef _H_CONFIGURATION
#define _H_CONFIGURATION

#include <SupportDefs.h>

#include "ButtonDownCommand.h"

#define SCROLL_MOUSE_DOWN_MASK 7

class Configuration {
	friend class UniversalScroller;
	friend class MainWindow;
	
public:
	// create and reload the configuration
	Configuration();	

	void Configuration::loadFallbackConfiguration( void );
	void load( void );
    void save( void );

    static int getButtonDownIndex( int32, int32 );

private:
	int32 minScroll;	
	int32 doubleClickSpeed[3];
	ButtonDownCommand buttonDownCommand[9];
	bool swallowClick[9];
		
	float wheelFactorX[2];
	float wheelFactorY[2];
	bool useWheelFactors;
	
	bool scrollWhenMouseDown[8];

};

#endif // _H_CONFIGURATION
