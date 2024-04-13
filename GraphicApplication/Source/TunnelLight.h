#ifndef TUNNELLIGHT_H
#define TUNNELLIGHT_H
#include "SaltnPepperEngine.h"

namespace SaltnPepperEngine
{
	class TunnelLight
	{
	private:

		Light* m_pointLight = nullptr;
		Audio::AudioSource* m_source = nullptr;
		AudioClip* m_glassclip = nullptr;

	public:

		void OnInit(Light* light, Audio::AudioSource* source, AudioClip* clip);

		void LightHit();
		

	};
}

#endif // !TUNNELLIGHT_H
