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
#include "ShaderDX11.h"
#include "BufferConfigDX11.h"
#include "Log.h"
#include "GlyphString.h"
#include <sstream>
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
ShaderDX11::ShaderDX11()
{
}
//--------------------------------------------------------------------------------
ShaderDX11::~ShaderDX11()
{

}
//--------------------------------------------------------------------------------
void ShaderDX11::UpdateParameters( RendererDX11* pRenderer )
{
	// Renderer will call this function when binding the shader to the pipeline.
	// This function will then call renderer functions to update each constant
	// buffer needed for the shader.

	for ( int i = 0; i < ConstantBuffers.count(); i++ )
	{
		if ( ConstantBuffers[i].Description.Type == D3D11_CT_CBUFFER )
		{
			// Get the index of the constant buffer parameter currently set with 
			// this name.
			int index = pRenderer->GetConstantBufferParameter( ConstantBuffers[i].Description.Name );

			// If the constant buffer does not exist yet, create a one with 
			// standard options - writeable by the CPU and only bound as a 
			// constant buffer.  By automatically creating the constant buffer
			// we reduce the amount of code to do common tasks, but still allow
			// the user to create and use a special buffer if they want.

			if ( index == -1 )
			{
				// Configure the buffer for the needed size and dynamic updating.
				BufferConfigDX11 cbuffer;
				cbuffer.SetDefaultConstantBuffer( ConstantBuffers[i].Description.Size, true );

				// Create the buffer and set it as a constant buffer parameter.  This
				// creates a parameter object to be used in the future.
				index = pRenderer->CreateConstantBuffer( &cbuffer, 0 );
				pRenderer->SetConstantBufferParameter( ConstantBuffers[i].Description.Name, &index );
			}

			// Map the constant buffer into system memory.  We map the buffer 
			// with the discard write flag since we don't care what was in the
			// buffer already.

			D3D11_MAPPED_SUBRESOURCE resource = 
				pRenderer->MapResource( index, 0, D3D11_MAP_WRITE_DISCARD, 0 );

			// Update each variable in the constant buffer
			for ( int j = 0; j < ConstantBuffers[i].Variables.count(); j++ )
			{
				std::wstring name = ConstantBuffers[i].Variables[j].Name;
				int offset = ConstantBuffers[i].Variables[j].StartOffset;
				UINT size = ConstantBuffers[i].Variables[j].Size;
				
				if ( ConstantBuffers[i].Types[j].Class == D3D10_SVC_VECTOR )
				{
					Vector4f vector = pRenderer->GetVectorParameter( name );
					Vector4f* pBuf = (Vector4f*)((char*)resource.pData + offset);
					*pBuf = vector;
				}
				else if ( ConstantBuffers[i].Types[j].Class == D3D10_SVC_MATRIX_ROWS )
				{
					Matrix4f matrix = pRenderer->GetMatrixParameter( name );
					Matrix4f* pBuf = (Matrix4f*)((char*)resource.pData + offset);
					*pBuf = matrix;
				}
				else if ( ConstantBuffers[i].Types[j].Class == D3D10_SVC_MATRIX_COLUMNS )
				{
					Matrix4f matrix = pRenderer->GetMatrixParameter( name );
					Matrix4f* pBuf = (Matrix4f*)((char*)resource.pData + offset);
					*pBuf = matrix;
				}
				else
				{
					Log::Get().Write( L"Non vector or matrix parameter specified in a constant buffer!  This will not be updated!" );
				}
			}

			pRenderer->UnMapResource( index, 0 );
		}
	}
}
//--------------------------------------------------------------------------------
void ShaderDX11::BindParameters( RendererDX11* pRenderer )
{
	// Here the shader will attempt to bind each parameter needed by the pipeline.
	// The renderer supplies methods for binding a parameter based on the name
	// and type of parameter, in addition to the pipeline stage to bind the 
	// parameter to.

	for ( int i = 0; i < ResourceBindings.count(); i++ )
	{
		std::wstring name = ResourceBindings[i].Name;
		UINT slot = ResourceBindings[i].BindPoint;

		switch ( ResourceBindings[i].Type )
		{
		case D3D10_SIT_CBUFFER:
			pRenderer->BindConstantBufferParameter( GetType(), name, slot );
			break;
		case D3D10_SIT_TBUFFER:
			pRenderer->BindConstantBufferParameter( GetType(), name, slot );
			break;
		case D3D10_SIT_TEXTURE:
			pRenderer->BindShaderResourceParameter( GetType(), name, slot );
			break;
		case D3D10_SIT_SAMPLER:
			pRenderer->BindSamplerStateParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_UAV_RWTYPED:
			pRenderer->BindUnorderedAccessParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_STRUCTURED:
			pRenderer->BindShaderResourceParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_UAV_RWSTRUCTURED:
			pRenderer->BindUnorderedAccessParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_BYTEADDRESS:
			pRenderer->BindShaderResourceParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_UAV_RWBYTEADDRESS:
			pRenderer->BindUnorderedAccessParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_UAV_APPEND_STRUCTURED:
			pRenderer->BindUnorderedAccessParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_UAV_CONSUME_STRUCTURED:
			pRenderer->BindUnorderedAccessParameter( GetType(), name, slot );
			break;
		case D3D11_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
			pRenderer->BindUnorderedAccessParameter( GetType(), name, slot );
			break;
		}
	}
}
//--------------------------------------------------------------------------------
void ShaderDX11::PrintShaderDetails()
{
	std::wstringstream s;

	s << L"----------------------------------------------------------------------" << std::endl;
	s << L"Shader details printout for: " << FileName << std::endl;
	s << L"----------------------------------------------------------------------" << std::endl;

	s << L"Shader Description: " << std::endl;
	s << RendererDX11::Print_D3D11_SHADER_DESC( this->ShaderDescription );
	
	s << std::endl;

	s << L"Number of Input Signature Elements: " << InputSignatureParameters.count() << std::endl;
	for ( int i = 0; i < InputSignatureParameters.count(); i++ )
	{
		s << L"  Semantic Name: " << InputSignatureParameters[i].SemanticName;
		s << L", Semantic Index: " << InputSignatureParameters[i].SemanticIndex;
		s << L", Register: " << InputSignatureParameters[i].Register;
		s << L", System Value Type: " << RendererDX11::TranslateSystemVariableName( InputSignatureParameters[i].SystemValueType ); 
		s << L", Component Type: " << RendererDX11::TranslateRegisterComponentType( InputSignatureParameters[i].ComponentType );
		s << L", Mask: " << (unsigned int)InputSignatureParameters[i].Mask;
		s << L", Read/Write Mask: " << (unsigned int)InputSignatureParameters[i].ReadWriteMask;
		s << std::endl;
	}
	s << std::endl;

	s << L"Number of Output Signature Elements: " << OutputSignatureParameters.count() << std::endl;
	for ( int i = 0; i < OutputSignatureParameters.count(); i++ )
	{
		s << L"  Semantic Name: " << OutputSignatureParameters[i].SemanticName;
		s << L", Semantic Index: " << OutputSignatureParameters[i].SemanticIndex;
		s << L", Register: " << OutputSignatureParameters[i].Register;
		s << L", System Value Type: " << RendererDX11::TranslateSystemVariableName( OutputSignatureParameters[i].SystemValueType );  
		s << L", Component Type: " << RendererDX11::TranslateRegisterComponentType( OutputSignatureParameters[i].ComponentType );
		s << L", Mask: " << (unsigned int)OutputSignatureParameters[i].Mask;
		s << L", Read/Write Mask: " << (unsigned int)OutputSignatureParameters[i].ReadWriteMask;
		s << std::endl;
	}
	s << std::endl;

	s << L"Number of Constant Buffer Descriptions: " << ConstantBuffers.count() << std::endl;
	for ( int i = 0; i < ConstantBuffers.count(); i++ )
	{
		s << L"  Buffer Name: " << ConstantBuffers[i].Description.Name;
		s << L", Buffer Type: " << RendererDX11::TranslateCBufferType( ConstantBuffers[i].Description.Type );
		s << L", Variables: " << ConstantBuffers[i].Description.Variables;
		s << L", Size: " << ConstantBuffers[i].Description.Size;
		s << L", Flags: " << ConstantBuffers[i].Description.uFlags;
		
		for ( int j = 0; j < ConstantBuffers[i].Variables.count(); j++ )
		{
			s << std::endl << L"    ";
			s << L"Variable Name: " << ConstantBuffers[i].Variables[j].Name;
			s << L", Start Offset: " << ConstantBuffers[i].Variables[j].StartOffset;
			s << L", Size: " << ConstantBuffers[i].Variables[j].Size;
			s << L", Flags: " << ConstantBuffers[i].Variables[j].uFlags;
			s << L", Start Texture: " << (int)ConstantBuffers[i].Variables[j].StartTexture;
			s << L", Texture Size: " << ConstantBuffers[i].Variables[j].TextureSize;
			s << L", Start Sampler: " << (int)ConstantBuffers[i].Variables[j].StartSampler;
			s << L", Sampler Size: " << ConstantBuffers[i].Variables[j].SamplerSize;
		}


		for ( int j = 0; j < ConstantBuffers[i].Types.count(); j++ )
		{
			s << std::endl << L"    ";
			s << L"Variable Type Name: " << ConstantBuffers[i].Types[j].Name;
			s << L", Class: " << RendererDX11::TranslateShaderVariableClass( ConstantBuffers[i].Types[j].Class );
			s << L", Type: " << RendererDX11::TranslateShaderVariableType( ConstantBuffers[i].Types[j].Type );			
			s << L", Rows: " << ConstantBuffers[i].Types[j].Rows;
			s << L", Columns: " << ConstantBuffers[i].Types[j].Columns;
			s << L", Elements: " << ConstantBuffers[i].Types[j].Elements;
			s << L", Members: " << ConstantBuffers[i].Types[j].Members;
			s << L", Offset: " << ConstantBuffers[i].Types[j].Offset;
		}
		
		s << std::endl;
	}
	s << std::endl;

	s << L"Number of Resource Binding Descriptions: " << ResourceBindings.count() << std::endl;
	for ( int i = 0; i < ResourceBindings.count(); i++ )
	{
		s << L"  Name: " << ResourceBindings[i].Name;
		s << L", Type: " << RendererDX11::TranslateShaderInputType( ResourceBindings[i].Type );
		s << L", Bind Point: " << ResourceBindings[i].BindPoint;
		s << L", Bind Count: " << ResourceBindings[i].BindCount;
		s << L", Flags: " << ResourceBindings[i].uFlags;
		s << L", Resource Return Type: " << RendererDX11::TranslateResourceReturnType( ResourceBindings[i].ReturnType );
		s << L", Dimension: " << ResourceBindings[i].Dimension;
		s << L", Number of Samples: " << ResourceBindings[i].NumSamples;
		s << std::endl;
	}

	s << std::endl;

	LPVOID pBlobBuffer = pCompiledShader->GetBufferPointer();
	const char* pMessage = (const char*)pBlobBuffer;
	
	s << pMessage << std::endl;

	s << L"----------------------------------------------------------------------" << std::endl;

	Log::Get().Write( s.str() );

}
//--------------------------------------------------------------------------------