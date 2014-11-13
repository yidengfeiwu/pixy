//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QImage>
#include "pixytypes.h"
//#include "processblobs.h"

#include <simplevector.h>
typedef SimpleVector<Point16> Points;

#define RAWFRAME_SIZE    0x10000

class Interpreter;

class VideoWidget;

class Renderer : public QObject
{
    Q_OBJECT

public:
    Renderer(VideoWidget *video, Interpreter *interpreter);
    ~Renderer();

    int render(uint32_t type, const void *args[]);
    int renderBackground();
    int renderRect(uint16_t width, uint16_t height, const RectA &rect);
    void emitFlushImage(bool blend=false);
    void emitImage(const QImage &image);
    QImage *backgroundImage(); // get background from BA81 formatted image data
    Frame8 *backgroundRaw();
    bool firstFrame(); // are we the first frame (default background) between flushes

    void setMode(uint32_t mode)
    {
        m_mode = mode;
    }

    int renderCCQ1(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t numVals, uint32_t *qVals);
    int renderBA81(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);
    int renderCCB1(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t numBlobs, uint16_t *blobs);
    int renderCCB2(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t numBlobs, uint16_t *blobs, uint32_t numCCBlobs, uint16_t *ccBlobs);
    int renderCMV1(uint8_t renderFlags, uint32_t cmodelsLen, float *cmodels, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);

    void renderBlobsB(QImage *image, float scale, BlobB *blobs, uint32_t numBlobs);
    void renderBlobsA(QImage *image, float scale, BlobA *blobs, uint32_t numBlobs);

    void renderRects(const Points &points, uint32_t size);
    void renderRect(const RectA &rect);
    int saveImage(const QString &filename);
    void pixelsOut(int x0, int y0, int width, int height);
    void renderRL(QImage *image, uint color, uint row, uint startCol, uint len);

    Frame8 m_rawFrame;
#ifdef DEFER
    ProcessBlobs m_blobs;
#endif

signals:
    void image(QImage image);
    void flushImage(bool blend);

private:
    inline void interpolateBayer(unsigned int width, unsigned int x, unsigned int y, unsigned char *pixel, unsigned int &r, unsigned int &g, unsigned int &b);



    VideoWidget *m_video;
    Interpreter *m_interpreter;

    bool m_firstFrame;
    QImage m_background;

    uint32_t m_mode;
};

#endif // RENDERER_H

