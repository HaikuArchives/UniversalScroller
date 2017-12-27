/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * Application.cpp - implementation of the application part of the 
 *   settings program for UniversalScroller
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
 
#include "Application.h"
#include "MainWindow.h"

Application::Application()
		  		  : BApplication("application/x-vnd.TM.USPref")
{
	(new MainWindow(BRect(50, 50, 420, 375)))->Show();
}
