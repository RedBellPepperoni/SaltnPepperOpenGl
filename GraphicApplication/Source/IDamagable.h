#ifndef I_DAMAGABLE_H
#define I_DAMAGABLE_H

namespace SaltnPepperEngine
{
	class IDamagable
	{
	public:

		enum class DamageSource : uint8_t
		{
			PLAYER,
			BUDDY,
			ZOMBIE,
			ENVIRONMENT
		};

		IDamagable() = default;
		virtual ~IDamagable() = default;


		virtual void TakeDamage(const int damage, const DamageSource& source) = 0;


		const int GetCurrentHealth() const { return m_health; }
		const int GetMaxHealth() const { return m_maxHealth; }

	protected:

		int m_maxHealth = 100;
		int m_health = 0;

	};


}

#endif // !I_DAMAGABLE_H
