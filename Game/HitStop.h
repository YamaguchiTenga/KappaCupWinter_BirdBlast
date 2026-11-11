#pragma once

class HitStop
{
public:
    void Start(float duration)
    {
        if (duration > 0.0f)
        {
            m_remainingTime = duration;
        }
        else
        {
            m_remainingTime = 0.0f;
        }
    }

    void Update(float deltaTime)
    {
        m_remainingTime -= deltaTime;
        if (m_remainingTime < 0.0f)
        {
            m_remainingTime = 0.0f;
        }
    }

    bool IsActive() const
    {
        return m_remainingTime > 0.0f;
    }

private:
    float m_remainingTime = 0.0f;
};