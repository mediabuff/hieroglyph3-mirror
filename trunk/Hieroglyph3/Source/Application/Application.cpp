//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) 2003-2010 Jason Zink 
//--------------------------------------------------------------------------------
#include "Application.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
// Initialize the application pointer to NULL
Application* Application::ms_pApplication = NULL;
//--------------------------------------------------------------------------------
Application::Application()
{
	// Record the this pointer to provide access to the WinMain function.

	ms_pApplication = this;

	// Create and initialize the timer object for use in the subclass.

	m_pTimer = new Timer();
	m_pTimer->Update();

	// Open the log file for use by the engine/application in general.

	Log::Get().Open();

	// Create the event manager here since it must be the first engine component
	// instantiated.

	m_pEventMgr = new EventManager();
}
//--------------------------------------------------------------------------------
Application::~Application( )
{
	if ( m_pTimer != NULL )
		delete m_pTimer;

	if ( m_pEventMgr )
		delete m_pEventMgr;

	Log::Get().Close();
}
//--------------------------------------------------------------------------------
Application* Application::GetApplication( )
{
	return( ms_pApplication );
}
//--------------------------------------------------------------------------------
void Application::RequestTermination( )
{
	// This triggers the termination of the application
	PostQuitMessage( 0 );
}
//--------------------------------------------------------------------------------