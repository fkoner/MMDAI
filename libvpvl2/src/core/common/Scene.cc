/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2012  hkrn                                    */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAI project team nor the names of     */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include "vpvl2/vpvl2.h"
#include "vpvl2/pmd/Model.h"
#include "vpvl2/pmx/Model.h"

#include "vpvl2/gl2/AssetRenderEngine.h"
#include "vpvl2/gl2/PMDRenderEngine.h"

namespace vpvl2
{

struct Scene::PrivateContext {
    PrivateContext()
        : encoding(0),
          lightColor(kZeroC),
          lightPosition(kZeroV3)
    {
    }
    ~PrivateContext() {
    }

    IEncoding *encoding;
    float modelViewProjectionMatrix[16];
    float modelViewMatrix[16];
    float projectionMatrix[16];
    float lightViewProjectionMatrix[16];
    float normalMatrix[9];
    Color lightColor;
    Vector3 lightPosition;
};

bool Scene::isAcceleratorSupported()
{
#ifdef VPVL2_ENABLE_OPENCL
    return true;
#else
    return false;
#endif
}

Scene::Scene(IEncoding *encoding)
    : m_context(0)
{
    m_context = new Scene::PrivateContext();
    m_context->encoding = encoding;
}

IModel *Scene::createModel(const uint8_t *data, size_t size, bool &ok) const
{
    IModel *model = 0;
    if (size >= 4 && memcmp(data, "PMX ", 4) == 0) {
        model = new vpvl2::pmx::Model(m_context->encoding);
    }
    else if (size >= 3 && memcmp(data, "Pmd", 3) == 0) {
        model = new vpvl2::pmd::Model(m_context->encoding);
    }
    ok = model ? model->load(data, size) : false;
    return model;
}

void Scene::setModelViewProjectionMatrix(const float value[])
{
    memcpy(m_context->modelViewProjectionMatrix, value, sizeof(m_context->modelViewProjectionMatrix));
}

IRenderEngine *Scene::createRenderEngine(IRenderDelegate *delegate, IModel *model) const
{
    IRenderEngine *engine = 0;
    engine = new gl2::PMDRenderEngine(delegate, this, static_cast<pmd::Model *>(model));
    return engine;
}

void Scene::setModelViewMatrix(const float value[16])
{
    memcpy(m_context->modelViewMatrix, value, sizeof(m_context->modelViewMatrix));
}

void Scene::setProjectionMatrix(const float value[16])
{
    memcpy(m_context->projectionMatrix, value, sizeof(m_context->projectionMatrix));
}

void Scene::setLightViewProjectionMatrix(const float value[16])
{
    memcpy(m_context->lightViewProjectionMatrix, value, sizeof(m_context->lightViewProjectionMatrix));
}

void Scene::setNormalMatrix(const float value[9])
{
    memcpy(m_context->normalMatrix, value, sizeof(m_context->normalMatrix));
}

void Scene::setLightColor(const Color &value)
{
    m_context->lightColor = value;
}

void Scene::setLightPosition(const Vector3 &value)
{
    m_context->lightPosition = value;
}

const float *Scene::modelViewProjectionMatrix() const
{
    return m_context->modelViewProjectionMatrix;
}

const float *Scene::modelViewMatrix() const
{
    return m_context->modelViewMatrix;
}

const float *Scene::projectionMatrix() const
{
    return m_context->projectionMatrix;
}

const float *Scene::lightViewProjectionMatrix() const
{
    return m_context->lightViewProjectionMatrix;
}

const float *Scene::normalMatrix() const
{
    return m_context->normalMatrix;
}

const Color &Scene::lightColor() const
{
    return m_context->lightColor;
}

const Vector3 &Scene::lightPosition() const
{
    return m_context->lightPosition;
}

}