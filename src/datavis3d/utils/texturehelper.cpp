/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDataVis3D module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "texturehelper_p.h"
#include "utils_p.h"

#include <QImage>

#include <QDebug>

QT_DATAVIS3D_BEGIN_NAMESPACE

TextureHelper::TextureHelper()
{
    initializeOpenGLFunctions();
}

TextureHelper::~TextureHelper()
{
}

GLuint TextureHelper::create2DTexture(const QImage &image, bool useTrilinearFiltering, bool convert)
{
    if (image.isNull())
        return 0;

    QImage texImage;

#if defined(Q_OS_ANDROID)
    GLuint temp;
    //qDebug() << "old size" << image.size();
    GLuint imageWidth = Utils::getNearestPowerOfTwo(image.width(), temp);
    //qDebug() << "new width" << imageWidth << "padding" << temp;
    GLuint imageHeight = Utils::getNearestPowerOfTwo(image.height(), temp);
    //qDebug() << "new height" << imageHeight << "padding" << temp;
    texImage = image.scaled(imageWidth, imageHeight, Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation);
    //qDebug() << "new size" << texImage.size();
#else
    texImage = image;
#endif

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (convert) {
        QImage glTexture = convertToGLFormat(texImage);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glTexture.width(), glTexture.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, glTexture.bits());
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.bits());
    }
    if (useTrilinearFiltering) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

GLuint TextureHelper::createCubeMapTexture(const QImage &image, bool useTrilinearFiltering)
{
    if (image.isNull())
        return 0;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    QImage glTexture = convertToGLFormat(image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGBA, glTexture.width(), glTexture.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, glTexture.bits());
    if (useTrilinearFiltering) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    } else {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

GLuint TextureHelper::createSelectionBuffer(const QSize &size, GLuint &texture,
                                            GLuint &depthTexture)
{
    GLuint framebuffer;

    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create texture for the selection buffer
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//#if !defined(QT_OPENGL_ES_2)
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width(), size.height(), 0, GL_RGB,
//                 GL_UNSIGNED_INT, NULL);
//#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width(), size.height(), 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
//#endif

    // Create texture object for the depth buffer
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.width(), size.height(),
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    // Attach texture to depth attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // Verify that the frame buffer is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        qCritical() << "Frame buffer creation failed" << status;
        return 0;
    }

    // Restore the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}

GLuint TextureHelper::createSelectionTexture(const QSize &size, GLuint &frameBuffer,
                                             GLuint &depthBuffer)
{
    GLuint textureid;

    // Create texture for the selection buffer
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//#if !defined(QT_OPENGL_ES_2)
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width(), size.height(), 0, GL_RGB,
//                 GL_UNSIGNED_INT, NULL);
//#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width(), size.height(), 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
//#endif
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create render buffer
    if (!depthBuffer)
        glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
#if !defined(QT_OPENGL_ES_2)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width(), size.height());
#else
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.width(), size.height());
#endif
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Create frame buffer
    if (!frameBuffer)
        glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Attach texture to color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureid, 0);
    // Attach renderbuffer to depth attachment
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    // Verify that the frame buffer is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        qCritical() << "Frame buffer creation failed" << status;
        return 0;
    }

    // Restore the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return textureid;
}

#if !defined(QT_OPENGL_ES_2)
GLuint TextureHelper::createDepthTexture(const QSize &size, GLuint &frameBuffer, GLuint textureSize)
{
    GLuint depthtextureid;

    // Create depth texture for the shadow mapping
    glGenTextures(1, &depthtextureid);
    glBindTexture(GL_TEXTURE_2D, depthtextureid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, size.width() * textureSize,
                 size.height() * textureSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create frame buffer
    if (!frameBuffer)
        glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Attach texture to depth attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtextureid, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Verify that the frame buffer is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        qCritical() << "Frame buffer creation failed" << status;
        return 0;
    }

    // Restore the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return depthtextureid;
}
#endif

void TextureHelper::deleteTexture(const GLuint *texture)
{
    glDeleteTextures(1, texture);
}

QImage TextureHelper::convertToGLFormat(const QImage &srcImage)
{
    QImage res(srcImage.size(), QImage::Format_ARGB32);
    convertToGLFormatHelper(res, srcImage.convertToFormat(QImage::Format_ARGB32), GL_RGBA);
    return res;
}

void TextureHelper::convertToGLFormatHelper(QImage &dstImage, const QImage &srcImage,
                                            GLenum texture_format)
{
    Q_ASSERT(dstImage.depth() == 32);
    Q_ASSERT(srcImage.depth() == 32);

    if (dstImage.size() != srcImage.size()) {
        int target_width = dstImage.width();
        int target_height = dstImage.height();
        qreal sx = target_width / qreal(srcImage.width());
        qreal sy = target_height / qreal(srcImage.height());

        quint32 *dest = (quint32 *) dstImage.scanLine(0); // NB! avoid detach here
        uchar *srcPixels = (uchar *) srcImage.scanLine(srcImage.height() - 1);
        int sbpl = srcImage.bytesPerLine();
        int dbpl = dstImage.bytesPerLine();

        int ix = int(0x00010000 / sx);
        int iy = int(0x00010000 / sy);

        quint32 basex = int(0.5 * ix);
        quint32 srcy = int(0.5 * iy);

        // scale, swizzle and mirror in one loop
        while (target_height--) {
            const uint *src = (const quint32 *) (srcPixels - (srcy >> 16) * sbpl);
            int srcx = basex;
            for (int x=0; x<target_width; ++x) {
                dest[x] = qt_gl_convertToGLFormatHelper(src[srcx >> 16], texture_format);
                srcx += ix;
            }
            dest = (quint32 *)(((uchar *) dest) + dbpl);
            srcy += iy;
        }
    } else {
        const int width = srcImage.width();
        const int height = srcImage.height();
        const uint *p = (const uint*) srcImage.scanLine(srcImage.height() - 1);
        uint *q = (uint*) dstImage.scanLine(0);

#if !defined(QT_OPENGL_ES_2)
        if (texture_format == GL_BGRA) {
#else
        if (texture_format == GL_BGRA8_EXT) {
#endif
            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                // mirror + swizzle
                for (int i=0; i < height; ++i) {
                    const uint *end = p + width;
                    while (p < end) {
                        *q = ((*p << 24) & 0xff000000)
                                | ((*p >> 24) & 0x000000ff)
                                | ((*p << 8) & 0x00ff0000)
                                | ((*p >> 8) & 0x0000ff00);
                        p++;
                        q++;
                    }
                    p -= 2 * width;
                }
            } else {
                const uint bytesPerLine = srcImage.bytesPerLine();
                for (int i=0; i < height; ++i) {
                    memcpy(q, p, bytesPerLine);
                    q += width;
                    p -= width;
                }
            }
        } else {
            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                for (int i=0; i < height; ++i) {
                    const uint *end = p + width;
                    while (p < end) {
                        *q = (*p << 8) | ((*p >> 24) & 0xff);
                        p++;
                        q++;
                    }
                    p -= 2 * width;
                }
            } else {
                for (int i=0; i < height; ++i) {
                    const uint *end = p + width;
                    while (p < end) {
                        *q = ((*p << 16) & 0xff0000) | ((*p >> 16) & 0xff) | (*p & 0xff00ff00);
                        p++;
                        q++;
                    }
                    p -= 2 * width;
                }
            }
        }
    }
}

QRgb TextureHelper::qt_gl_convertToGLFormatHelper(QRgb src_pixel, GLenum texture_format)
{
#if !defined(QT_OPENGL_ES_2)
    if (texture_format == GL_BGRA) {
#else
    if (texture_format == GL_BGRA8_EXT) {
#endif
        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
            return ((src_pixel << 24) & 0xff000000)
                    | ((src_pixel >> 24) & 0x000000ff)
                    | ((src_pixel << 8) & 0x00ff0000)
                    | ((src_pixel >> 8) & 0x0000ff00);
        } else {
            return src_pixel;
        }
    } else {  // GL_RGBA
        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
            return (src_pixel << 8) | ((src_pixel >> 24) & 0xff);
        } else {
            return ((src_pixel << 16) & 0xff0000)
                    | ((src_pixel >> 16) & 0xff)
                    | (src_pixel & 0xff00ff00);
        }
    }
}

QT_DATAVIS3D_END_NAMESPACE