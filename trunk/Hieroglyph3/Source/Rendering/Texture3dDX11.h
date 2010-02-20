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
// Texture3dDX11
//
//--------------------------------------------------------------------------------
#include "ResourceDX11.h"
//--------------------------------------------------------------------------------
#ifndef Texture3dDX11_h
#define Texture3dDX11_h
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class Texture3dDX11 : public ResourceDX11
	{
	public:
		Texture3dDX11( ID3D11Texture3D* pTex );
		virtual ~Texture3dDX11();

		D3D11_TEXTURE3D_DESC		GetActualDescription();
		D3D11_TEXTURE3D_DESC		GetDesiredDescription();
		void						SetDesiredDescription( D3D11_TEXTURE3D_DESC description );

		virtual D3D11_RESOURCE_DIMENSION	GetType();
		virtual ID3D11Resource*				GetResource();

		virtual UINT						GetEvictionPriority();
		virtual void						SetEvictionPriority( UINT EvictionPriority );

	protected:
		ID3D11Texture3D*			m_pTexture;
		D3D11_TEXTURE3D_DESC		m_DesiredDesc;
		D3D11_TEXTURE3D_DESC		m_ActualDesc;

		friend RendererDX11;
	};
};
//--------------------------------------------------------------------------------
#endif // Texture3dDX11_h
//--------------------------------------------------------------------------------
