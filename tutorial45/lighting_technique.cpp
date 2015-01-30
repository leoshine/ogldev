/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <limits.h>
#include <string.h>

#include "ogldev_math_3d.h"
#include "ogldev_util.h"
#include "lighting_technique.h"


LightingTechnique::LightingTechnique()
{   
}


bool LightingTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "shaders/lighting.vs")) {
        return false;
    }


    if (!AddShader(GL_FRAGMENT_SHADER, "shaders/lighting.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

	m_posTextureUnitLocation = GetUniformLocation("gPositionMap");
	//m_normalTextureUnitLocation = GetUniformLocation("gNormalMap");
    m_randomTextureUnitLocation = GetUniformLocation("gRandomMap");
    m_depthTextureUnitLocation = GetUniformLocation("gDepthMap");
    m_screenSizeLocation = GetUniformLocation("gScreenSize");	
    m_AILocation = GetUniformLocation("gIntensity");
    m_sampleRadLocation = GetUniformLocation("gSampleRad");
    m_projMatrixLocation = GetUniformLocation("gProj");
    m_kernelLocation = GetUniformLocation("gKernel");

    if (m_posTextureUnitLocation    == INVALID_UNIFORM_LOCATION ||
		m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_randomTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_depthTextureUnitLocation  == INVALID_UNIFORM_LOCATION ||
        m_screenSizeLocation        == INVALID_UNIFORM_LOCATION ||
        m_AILocation                == INVALID_UNIFORM_LOCATION ||
        m_sampleRadLocation         == INVALID_UNIFORM_LOCATION ||
        m_projMatrixLocation        == INVALID_UNIFORM_LOCATION ||
        m_kernelLocation            == INVALID_UNIFORM_LOCATION) {
    //    return false;
    }

 /*   for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_kernelLocation) ; i++ ) {
        char Name[128] = { 0 };
        SNPRINTF(Name, sizeof(Name), "gkernel[%d]", i);        
        m_kernelLocation[i] = GetUniformLocation("gKernel");
        
        if (m_kernelLocation[i] == INVALID_UNIFORM_LOCATION) {
            return false;
        }
    }*/
    
    Enable();
    
    GLExitIfError;
    
    GenKernel();
    
    GLExitIfError;
    
    return GLCheckError();
}


void LightingTechnique::GenKernel()
{
    Vector3f kernel[KERNEL_SIZE];
    
    for (uint i = 0 ; i < KERNEL_SIZE ; i++ ) {
        float scale = (float)i / (float)(KERNEL_SIZE);        
        Vector3f v;
        v.x = 2.0f * (float)rand()/RAND_MAX - 1.0f;
        v.y = 2.0f * (float)rand()/RAND_MAX - 1.0f;
        v.z = (float)rand()/RAND_MAX;
        v *= (0.1f + 0.9f * scale * scale);
        
        v.Print();
        printf("\n");
        kernel[i] = v;
        //glUniform3f(m_kernelLocation[i], v.x, v.y, v.z);
    }
       
    glUniform3fv(m_kernelLocation, KERNEL_SIZE, (const GLfloat*)&kernel[0]);    
}


void LightingTechnique::SetPositionTextureUnit(uint TextureUnit)
{
    glUniform1i(m_posTextureUnitLocation, TextureUnit);
}


void LightingTechnique::SetNormalTextureUnit(uint TextureUnit)
{
    glUniform1i(m_normalTextureUnitLocation, TextureUnit);
}


void LightingTechnique::SetRandomTextureUnit(uint TextureUnit)
{
    glUniform1i(m_randomTextureUnitLocation, TextureUnit);
}


void LightingTechnique::SetDepthTextureUnit(uint TextureUnit)
{
    glUniform1i(m_depthTextureUnitLocation, TextureUnit);
}


void LightingTechnique::SetScreenSize(uint Width, uint Height)
{
    glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
}


void LightingTechnique::SetAmbientIntensity(float ai)
{
    glUniform1f(m_AILocation, ai);
}


void LightingTechnique::SetSampleRadius(float sr)
{
    glUniform1f(m_sampleRadLocation, sr);
}


void LightingTechnique::SetProjMatrix(const Matrix4f& m)
{
    glUniformMatrix4fv(m_projMatrixLocation, 1, GL_TRUE, (const GLfloat*)m.m);    
}
