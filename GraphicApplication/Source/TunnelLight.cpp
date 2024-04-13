#include "TunnelLight.h"

void SaltnPepperEngine::TunnelLight::OnInit(Light* light, Audio::AudioSource* source, AudioClip* clip)
{
	m_pointLight = light;
	m_source = source;
	m_glassclip = clip;
}

void SaltnPepperEngine::TunnelLight::LightHit()
{
	m_pointLight->intensity = 0.0f;
	m_source->PlayOneShot(m_glassclip);
}
