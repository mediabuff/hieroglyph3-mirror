//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) 2003-2010 Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "EvtFrameStart.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
EvtFrameStart::EvtFrameStart( )
{
}
//--------------------------------------------------------------------------------
EvtFrameStart::~EvtFrameStart( )
{
}
//--------------------------------------------------------------------------------
std::wstring EvtFrameStart::GetEventName( )
{
	return( std::wstring( L"frame_start" ) );
}
//--------------------------------------------------------------------------------
eEVENT EvtFrameStart::GetEventType( )
{
	return( RENDER_FRAME_START );
}
//--------------------------------------------------------------------------------