/******************************************************************************
* Copyright (c) 2018(-2021) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.18.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

/**
 * @file touchgfx/widgets/VideoWidget.hpp
 *
 * Declares the touchgfx::VideoWidget class.
 */
#ifndef TOUCHGFX_VIDEOWIDGET_HPP
#define TOUCHGFX_VIDEOWIDGET_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/hal/VideoController.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
class VideoWidget;

/**
 * A Widget for displaying video.
 *
 * The Widget interacts with a VideoController instance.
 *
 * @see VideoController
 */
class VideoWidget : public Widget
{
public:
    /** Default constructor. */
    VideoWidget();

    /** Destructor. Unregisters the Widget from the Controller. */
    ~VideoWidget();

    /** Play the video. */
    void play();

    /** Pause the video. */
    void pause();

    /** Stop the video. */
    void stop();

    /**
     * Check if the video is playing (not paused or stopped).
     *
     * @return  Returns true if the video is playing.
     */
    bool isPlaying();

    /**
     * Set repeat mode. When set the video is restarted when the end is reached.
     *
     * @param   repeat  When true, the video is repeated.
     */
    void setRepeat(bool repeat);

    /**
     * Seek to specific frame. Frame number 1 is the first frame.
     * The display is not updated updated unless the video is playing.
     *
     * @param   frameNumber The frame number to seek to.
     */
    void seek(uint32_t frameNumber);

    /**
     * Get the current frame number.
     *
     * @return  Returns the current frame number.
     */
    uint32_t getCurrentFrameNumber();

    /**
     * Associates an action to be performed when the movie has ended. If the video is set to repeat,
     * the action is also triggered when the animation starts over.
     *
     * @param   callback    The callback is executed when done. The callback is given the VideoWidget.
     */
    void setMovieEndedAction(GenericCallback<const VideoWidget&>& callback)
    {
        movieEndedAction = &callback;
    }

    /**
     * Clears the movie ended action previously set by setMovieEndedAction.
     *
     * @see setMovieEndedAction
     */
    void clearMovieEndedAction()
    {
        movieEndedAction = 0;
    }

    /**
     * Sets the frame rate of the video.
     *
     * To get 20 video frames pr second on a 60 fps display use video_frames = 20 and ui_frames = 60.
     *
     * @param   ui_frames       Number of UI frames (divider)
     * @param   video_frames    Number of video_frames (dividend)
     */
    void setFrameRate(uint32_t ui_frames, uint32_t video_frames);

    /**
     * Set the video data for the stream.
     * The video is paused and set to start on the first frame.
     *
     * @param   movie   Pointer to the video data.
     * @param   length  Length of the vide data.
     */
    void setVideoData(const uint8_t* movie, const uint32_t length);

    /**
     * Set the video data for the stream.
     * The video is paused and set to start on the first frame.
     *
     * @param [in,out]  reader  Reference to a VideoDataReader object.
     */
    void setVideoData(VideoDataReader& reader);

    /**
     * Get Video information.
     *
     * Get information from the video data.
     *
     * @param [in,out]  data    Pointer to VideoInformation where information should be stored.
     */
    void getVideoInformation(VideoInformation* data);

    /**
     * Set video buffer data.
     * Only used when video frames are decoded to a buffer and not directly to the framebuffer.
     *
     * @param [in] videoBuffer Video buffer.
     */
    void setVideoBuffer(uint8_t* const videoBuffer)
    {
        buffer = videoBuffer;
    }

    /**
     * Set video buffer format.
     * Only used when video frames are decoded to a buffer and not directly to the framebuffer.
     *
     * @param bufferFormat Format of the videoBuffer (RGB565 or RGB888)
     * @param width  Width of the videoBuffer in pixels
     * @param height Height of the videoBuffer in pixels
     */
    void setVideoBufferFormat(Bitmap::BitmapFormat bufferFormat, uint16_t width, uint16_t height)
    {
        format = bufferFormat;
        bufferWidth = width;
        bufferHeight = height;
    }

    virtual void handleTickEvent();

    virtual void draw(const Rect& invalidatedArea) const;

    virtual Rect getSolidRect() const;

private:
    /**
     * Reads information from the video. Sets the video width and height.
     * Sets the framerate to speed specified in movie. Assumes 60 ui
     * frame pr. second.
     */
    void readVideoInformation();

    VideoController::Handle handle;                        ///< The handle of this video stream
    GenericCallback<const VideoWidget&>* movieEndedAction; ///< Pointer to the callback to be executed when the video is done.
    uint8_t* buffer;                                       ///< The buffer where the pixels are copied from
    Bitmap::BitmapFormat format;                           ///< The pixel format for the data.
    uint16_t bufferWidth;                                  ///< Width (stride) of buffer in pixels (when used)
    uint16_t bufferHeight;                                 ///< Height of buffer in pixels (when used)
    uint16_t videoWidth;                                   ///< Width of video in pixels
    uint16_t videoHeight;                                  ///< Height of video in pixels
};

} // namespace touchgfx

#endif // TOUCHGFX_VIDEOWIDGET_HPP
