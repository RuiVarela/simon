#pragma once

#include "Project.h"

namespace re
{
	class Timer
	{
	public:
		typedef Uint64 TickType;
		typedef Uint64 OperationCount;

		Timer() 
			:start_tick(0), seconds_per_tick(0.0)
		{
			start();
		}

		~Timer(){ }

		// Starts the timer
		void start();

		// total time elapse since start();
		Real64 elapsedTime() const
		{
			return (Real64) (getTick() - start_tick) * seconds_per_tick;
		}

		Real64 elapsedTimeMilliseconds() const
		{
			return elapsedTime() * 1000.0;
		}

		// Operation time
		Real64 operationTime() const
		{
			return operation_time_;
		}

		// advances one operation
		Real64 advanceOperation()
		{
			Real64 current_time = elapsedTime();
			operation_time_ = current_time - last_time_;
			++operation_;
			last_time_ = current_time;
			return current_time;
		}

		// time elapsed since operation start
		Real64 elapsedSinceOperationStart() const
		{
			return elapsedTime() - last_time_;
		}

		// current operation number
		OperationCount operation() const { return operation_; }

		// calculates the current fps
		Int getFps() const
		{
			if (equivalent(operation_time_, 0.0))
			{
				return INT_MAX;
			}
			return Int(1.0 / operation_time_);
		}

	private:
		TickType getTick() const;

		void initialize()
		{
			last_time_= 0.0;
			operation_time_ = 0.0;
			operation_ = 0;
		}

		Real64 last_time_;
		Real64 operation_time_;
		OperationCount operation_;
		TickType start_tick;
		Real64 seconds_per_tick;
	};


    Int64 getCurrentMs();

	class ElapsedTimer 
	{
		public:
			ElapsedTimer() 
			{
				invalidate();
			}

			void start()
			{
				restart();
			}

			Int64 restart() 
			{
				Int64 value = elapsed();
				start_ts = timer.elapsedTime();
				return value;
			}

			void invalidate() 
			{
				start_ts = -1.0;
			}

			bool isValid() const 
			{
				return start_ts >= 0.0;
			}

			Int64 elapsed() const 
			{
				return Int64( (timer.elapsedTime() - start_ts) * 1000.0 );
			}

			bool hasExpired(Int64 timeout) const {
				// if timeout is -1, quint64(timeout) is LLINT_MAX, so this will be
				// considered as never expired
				return Uint64(elapsed()) > Uint64(timeout);
			}
		private:
			Timer timer;
			Real64 start_ts;
	};



    class DurationProbe {
    public:
        DurationProbe(size_t samples = 25, std::string const& tag = "DurationProbe");
        ~DurationProbe();
        void setTag(std::string const& tag);
        void setPrefix(std::string const& prefix);

        void Start();
        void Stop(int warningInterval = 5000);
    private:
        MeanBufferFloat m_time_mean; // measures the time between start and stop calls
        MeanBufferFloat m_starts_time_mean; // measures the time between start calls
        float m_time_max;
        float m_time_min;
        std::chrono::high_resolution_clock::time_point m_start;
        ElapsedTimer m_elased_timer;
        std::string m_prefix;
        std::string TAG;
    };

} // end of namespace

