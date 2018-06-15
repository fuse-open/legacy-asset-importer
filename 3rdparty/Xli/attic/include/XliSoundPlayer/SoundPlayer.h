/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_AUDIO_SIMPLE_AUDIO_PLAYER_H__
#define __XLI_AUDIO_SIMPLE_AUDIO_PLAYER_H__

#include <Xli/Object.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliSoundPlayer
    */
    class SoundChannel : public Object
    {
    public:
        virtual void Pause() = 0;
        virtual void UnPause() = 0;
        virtual double GetPosition() const = 0;
        virtual void SetPosition(double position) = 0;
        virtual float GetVolume() const = 0;
        virtual void SetVolume(float volume) const = 0;
        virtual bool IsPlaying() = 0;
        virtual bool IsFinished() = 0;

        virtual double GetDuration() const = 0;
        virtual float GetPan() const = 0;
        virtual void SetPan(float pan) const = 0;
        virtual void Play() = 0;
        virtual void Stop() = 0;
        virtual bool IsPaused() = 0;
    };

    /**
        \ingroup XliSoundPlayer
    */
    class Sound : public Object
    {
    public:
        virtual double GetDuration() const = 0;
        virtual String GetPath() const = 0;
        virtual bool IsAsset() const = 0;
    };

    /**
        \ingroup XliSoundPlayer
    */
    class SoundPlayer: public Object
    {
    public:
        virtual Sound* CreateSoundFromAsset(const String& filename) = 0;
        virtual SoundChannel* PlaySound(Sound* sound, bool loop) = 0;
        virtual void Update() {}
        static SoundPlayer* Create();
    };
}

#endif
