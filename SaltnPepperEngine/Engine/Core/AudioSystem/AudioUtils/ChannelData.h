#ifndef CHANNELDATA_H
#define CHANNELDATA_H

#include <functional>

namespace SaltnPepperEngine
{
	namespace Audio
	{

		struct ChannelData
		{
			ChannelData() = default;
			ChannelData(int initchannelId, bool initpaused, bool initmuted, const std::function<void()> initstopCallback)
				: channelId(initchannelId)
				, paused(initpaused)
				, muted(initmuted)
				, stopCallback(stopCallback)
			{

			}

			int channelId = 0;
			bool paused = false;
			bool muted = false;
			std::function<void()> stopCallback; 
		
		};


		
	}
}

#endif // !CHANNELDATA_H

