/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NET_TIMER_TIMER_H
#define NET_TIMER_TIMER_H

#include "core/TimerEventReceiver.h"

namespace core::timer {
    class IntervalTimer;
    class SingleshotTimer;
} // namespace core::timer

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core::timer {

    class Timer : public TimerEventReceiver {
    protected:
        using TimerEventReceiver::TimerEventReceiver;

        virtual ~Timer() = default;

    public:
        static IntervalTimer& intervalTimer(const std::function<void(const void*, const std::function<void()>& stop)>& dispatcher,
                                            const struct timeval& timeout,
                                            const void* arg);

        static IntervalTimer&
        intervalTimer(const std::function<void(const void*)>& dispatcher, const struct timeval& timeout, const void* arg);

        static SingleshotTimer&
        singleshotTimer(const std::function<void(const void*)>& dispatcher, const struct timeval& timeout, const void* arg);

        void cancel();

    protected:
        void unobservedEvent() override;

        ttime::Timeval getTimeout() const override;

        bool operator<(const TimerEventReceiver& timerEventReceiver) const override;
    };

} // namespace core::timer

#endif // NET_TIMER_TIMER_H
