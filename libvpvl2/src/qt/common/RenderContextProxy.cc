/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2013  hkrn                                    */
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

#include "vpvl2/qt/RenderContextProxy.h"
#include "vpvl2/extensions/gl/FrameBufferObject.h"

#include <QtCore/QtCore>

namespace vpvl2
{
namespace qt
{

void RenderContextProxy::initialize()
{
#ifdef VPVL2_LINK_GLEW
    glewInit();
#endif
}

FrameBufferObject *RenderContextProxy::newFrameBufferObject(size_t width, size_t height, int samples)
{
    return new FrameBufferObject(width, height, samples);
}

FrameBufferObject *RenderContextProxy::createFrameBufferObject(size_t width, size_t height, int samples, bool enableAA)
{
    FrameBufferObject *fbo = new FrameBufferObject(width, height, samples);
    fbo->create(enableAA);
    return fbo;
}

void RenderContextProxy::getMSAASamples(int *nsamples)
{
    glGetIntegerv(GL_MAX_SAMPLES, nsamples);
}

void RenderContextProxy::setRenderTargets(const void *targets, size_t ntargets)
{
    glDrawBuffers(ntargets, static_cast<const GLenum *>(targets));
}

void RenderContextProxy::bindOffscreenRenderTarget(unsigned int textureID, unsigned int textureFormat, FrameBufferObject *fbo)
{
    if (fbo) {
        fbo->bindTexture(textureID, textureFormat, 0);
        fbo->bindDepthStencilBuffer();
    }
}

void RenderContextProxy::releaseOffscreenRenderTarget(FrameBufferObject *fbo)
{
    if (fbo) {
        fbo->transferMSAABuffer(0);
        fbo->unbindColorBuffer(0);
        fbo->unbindDepthStencilBuffer();
        fbo->unbind();
    }
}

void RenderContextProxy::deleteAllRenderTargets(QHash<unsigned int, FrameBufferObject *> &renderTargets)
{
    qDeleteAll(renderTargets);
    renderTargets.clear();
}

}
}