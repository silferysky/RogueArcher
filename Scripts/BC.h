class BC
{
	Vec2 m_centre;
	float m_radius;
public:
	BC(Vec2 centre, float radius) : m_centre{ centre }, m_radius{ radius }
	{}
	
	~BC() = default;

	Vec2 getCentre() const;
	float getRadius() const;
	void setCentre(const float x, const float y);
	void setRadius(const float radius);
};