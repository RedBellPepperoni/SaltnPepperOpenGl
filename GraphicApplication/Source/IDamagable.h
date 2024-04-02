#ifndef I_DAMAGABLE_H
#define I_DAMAGABLE_H

namespace SaltnPepperEngine
{
	class IDamagable
	{
	public:

		IDamagable() = default;
		virtual ~IDamagable() = default;


		virtual void TakeDamage(const int damage) = 0;


		const int GetCurrentHealth() const { return m_health; }
		const int GetMaxHealth() const { return m_maxHealth; }

	protected:

		int m_maxHealth = 100;
		int m_health = 0;

	};


}

#endif // !I_DAMAGABLE_H
