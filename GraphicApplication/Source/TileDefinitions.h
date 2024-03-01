#ifndef TILEDEFINITIONS_H
#define TILEDEFINITIONS_H

namespace SaltnPepperEngine
{
	class Tile
	{
	public:

		Tile() = default;

		Tile(int x, int y, float weight)
		{
			m_Xposition = x;
			m_Yposition = y;
			m_weight = weight;
		}

		virtual ~Tile() {};

		inline const float GetWeight() const { return m_weight; }
		inline void SetWeight(const float& newWeight) { m_weight = newWeight; }

		inline const int GetXPos() const { return m_Xposition; }
		inline const int GetYPos() const { return m_Yposition; }

		inline void SetXPos(int newPos) { m_Xposition = newPos; }
		inline void SetYPos(int newPos) { m_Yposition = newPos; }

		bool operator==(const Tile& other) const
		{
			return (GetXPos() == other.GetXPos()) && (GetYPos() == other.GetYPos());
		}

	protected:

		int m_Xposition = 0;
		int m_Yposition = 0;
		float m_weight = 0;
	};


	class Treasure : public Tile
	{
		Treasure(int x, int y)
		{
			m_Xposition = x;
			m_Yposition = y;
		}

		virtual ~Treasure() = default;

		inline const bool GetPicked() const { return m_picked; }
		inline void SetPicked(const bool newPicked = true) { m_picked = newPicked; }

	private:
		bool m_picked = false;
	};


}

#endif // !TILEDEFINITIONS_H
