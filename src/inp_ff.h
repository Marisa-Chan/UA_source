#ifndef INPUT_FFEEDBACK_H_INCLUDE
#define INPUT_FFEEDBACK_H_INCLUDE

#include <SDL2/SDL_haptic.h>

namespace Input { namespace FF {
    
class FFEffect
{
public:
    FFEffect()
    {
        effectID = -1;
        haptic = NULL;
        SDL_memset( &descr, 0, sizeof(descr) );
    }

    virtual ~FFEffect()
    {
        Unbind();
    }

    virtual void Bind(SDL_Haptic *dev)
    {
        if (haptic)
            Unbind();

        if (dev)
        {
            if ( SDL_HapticEffectSupported(dev, &descr) == SDL_TRUE )
            {
                effectID = SDL_HapticNewEffect(dev, &descr);

                if (effectID >= 0)
                    haptic = dev;
            }
        }
    }

    virtual void Unbind()
    {
        if (haptic)
        {
            if (effectID >= 0)
                SDL_HapticDestroyEffect(haptic, effectID);

            effectID = -1;
            haptic = NULL;
        }
    }

    virtual void Run()
    {
        if (OK())
            SDL_HapticRunEffect(haptic, effectID, 1);
    }

    virtual void Stop()
    {
        if (OK())
            SDL_HapticStopEffect(haptic, effectID);
    }

    virtual void UpdateHaptic(SDL_HapticEffect &data)
    {
        if (OK())
            SDL_HapticUpdateEffect(haptic, effectID, &data);
    }

    virtual bool OK()
    {
        return (haptic && effectID >= 0);
    }

protected:

    static void DefaultPeriodic(SDL_HapticEffect &eff)
    {
        eff.type = SDL_HAPTIC_SINE;

        eff.periodic.direction.type = SDL_HAPTIC_POLAR;
        eff.periodic.direction.dir[0] = 0;

        eff.periodic.length = SDL_HAPTIC_INFINITY;
        eff.periodic.delay = 0;

        eff.periodic.button = 0;
        eff.periodic.interval = 0;

        eff.periodic.attack_length = 50;
        eff.periodic.attack_level = 0;
        eff.periodic.fade_length = 50;
        eff.periodic.fade_level = 0;
    }


protected:
    SDL_HapticEffect descr;
    int effectID;
    SDL_Haptic *haptic;
};

class TankEngine: public FFEffect
{
public:
    TankEngine()
    {
        DefaultPeriodic(descr);

        descr.periodic.direction.dir[0] = 9000;

        descr.periodic.magnitude = 9830; //3000
        descr.periodic.offset = 0;
        descr.periodic.phase = 0;
        descr.periodic.period = 200;

        updater = descr;
    }

    void Update(int magnitude, int period)
    {
        updater.periodic.magnitude = magnitude;
        updater.periodic.period = period;

        FFEffect::UpdateHaptic(updater);
    }

protected:
    SDL_HapticEffect updater;
};

class JetEngine: public FFEffect
{
public:
    JetEngine()
    {
        DefaultPeriodic(descr);

        descr.periodic.direction.dir[0] = 9000;

        descr.periodic.magnitude = 7536; //2300;
        descr.periodic.offset = 0;
        descr.periodic.phase = 0;
        descr.periodic.period = 71;

        updater = descr;
    }

    void Update(int magnitude, int period)
    {
        updater.periodic.magnitude = magnitude;
        updater.periodic.period = period;

        FFEffect::UpdateHaptic(updater);
    }

protected:
    SDL_HapticEffect updater;
};

class CopterEngine: public FFEffect
{
public:
    CopterEngine()
    {
        DefaultPeriodic(descr);

        descr.periodic.magnitude = 14745; //4500;
        descr.periodic.offset = 0;
        descr.periodic.phase = 0;
        descr.periodic.period = 167;

        updater = descr;
    }

    void Update(int magnitude, int period)
    {
        updater.periodic.magnitude = magnitude;
        updater.periodic.period = period;

        FFEffect::UpdateHaptic(updater);
    }

protected:
    SDL_HapticEffect updater;
};

class RotationDamper: public FFEffect
{
public:
    RotationDamper()
    {
        descr.type = SDL_HAPTIC_DAMPER;

        descr.condition.direction.type = SDL_HAPTIC_POLAR;
        descr.condition.direction.dir[0] = 0;

        descr.condition.length = SDL_HAPTIC_INFINITY;
        descr.condition.delay = 0;

        descr.condition.button = 0;
        descr.condition.interval = 0;

        descr.condition.center[0] = 0;
        descr.condition.right_coeff[0] = 0x7FFF;
        descr.condition.left_coeff[0] = 0x7FFF;
        descr.condition.right_sat[0] = 0;
        descr.condition.left_sat[0] = 0;
        descr.condition.deadband[0] = 0;
        descr.condition.center[1] = 0;
        descr.condition.right_coeff[1] = 0x7FFF;
        descr.condition.left_coeff[1] = 0x7FFF;
        descr.condition.right_sat[1] = 0;
        descr.condition.left_sat[1] = 0;
        descr.condition.deadband[1] = 0;

        updater = descr;
    }

    void Update(int coef)
    {
        updater.condition.right_coeff[0] = coef;
        updater.condition.left_coeff[0] = coef;
        updater.condition.right_coeff[1] = coef;
        updater.condition.left_coeff[1] = coef;

        FFEffect::UpdateHaptic(updater);
    }

protected:
    SDL_HapticEffect updater;
};

class MiniGun: public FFEffect
{
public:
    MiniGun()
    {
        DefaultPeriodic(descr);

        /* !!! FIXME: put this back when we have more bits in SDL 2.1 */
        //descr.type = SDL_HAPTIC_SQUARE;

        descr.periodic.magnitude = 16383; //5000;
        descr.periodic.offset = 0;
        descr.periodic.phase = 0;
        descr.periodic.period = 83;

        // Try emulate it with sine
        descr.periodic.attack_length = 0;
        descr.periodic.attack_level = 0;
        descr.periodic.fade_length = 0;
        descr.periodic.fade_level = 0;
    }
};

class MissileFire: public FFEffect
{
public:
    MissileFire()
    {
        DefaultPeriodic(descr);

        descr.periodic.length = 600;

        descr.periodic.magnitude = 32767; //10000;
        descr.periodic.offset = 0;
        descr.periodic.phase = 9000;
        descr.periodic.period = 1000;

        descr.periodic.attack_length = 0;
        descr.periodic.attack_level = 32767; //10000
        descr.periodic.fade_length = 264;
        descr.periodic.fade_level = 0;
    }
};

class GrenadeFire: public FFEffect
{
public:
    GrenadeFire()
    {
        descr.type = SDL_HAPTIC_RAMP;

        descr.ramp.direction.type = SDL_HAPTIC_POLAR;
        descr.ramp.direction.dir[0] = 0;

        descr.ramp.length = 300;
        descr.ramp.delay = 0;

        descr.ramp.button = 0;
        descr.ramp.interval = 0;

        descr.ramp.start = 32767; // 10000
        descr.ramp.end = -32767; // -10000

        descr.ramp.attack_length = 0;
        descr.ramp.attack_level = 32767; //10000
        descr.ramp.fade_length = 57;
        descr.ramp.fade_level = 0;
    }
};

class BombFire: public FFEffect
{
public:
    BombFire()
    {
        descr.type = SDL_HAPTIC_RAMP;

        descr.ramp.direction.type = SDL_HAPTIC_POLAR;
        descr.ramp.direction.dir[0] = 0;

        descr.ramp.length = 400;
        descr.ramp.delay = 0;

        descr.ramp.button = 0;
        descr.ramp.interval = 0;

        descr.ramp.start = 26213; // 8000
        descr.ramp.end = -26213; // -8000

        descr.ramp.attack_length = 0;
        descr.ramp.attack_level = 0;
        descr.ramp.fade_length = 0;
        descr.ramp.fade_level = 0;
    }
};

class Collision: public FFEffect
{
public:
    Collision()
    {
        descr.type = SDL_HAPTIC_RAMP;

        descr.ramp.direction.type = SDL_HAPTIC_CARTESIAN;
        descr.ramp.direction.dir[0] = 0;

        descr.ramp.length = 95;
        descr.ramp.delay = 0;

        descr.ramp.button = 0;
        descr.ramp.interval = 0;

        descr.ramp.start = 32767; // 10000
        descr.ramp.end = -32767; // -10000

        descr.ramp.attack_length = 0;
        descr.ramp.attack_level = 0;
        descr.ramp.fade_length = 0;
        descr.ramp.fade_level = 0;

        updater = descr;
    }

    void Update(float force, float x, float y)
    {
        updater.ramp.start = 32767 * force;
        updater.ramp.end = -32767 * force;

        updater.ramp.direction.dir[0] = x * 1000.0;
        updater.ramp.direction.dir[1] = y * 1000.0;

        FFEffect::UpdateHaptic(updater);
    }

protected:
    SDL_HapticEffect updater;
};

class Shake: public FFEffect
{
public:
    Shake()
    {
        DefaultPeriodic(descr);

        /* !!! FIXME: put this back when we have more bits in SDL 2.1 */
        //descr.type = SDL_HAPTIC_SQUARE;

        descr.periodic.direction.type = SDL_HAPTIC_CARTESIAN;

        descr.periodic.length = 1000;

        descr.periodic.magnitude = 32767; //10000
        descr.periodic.offset = 0;
        descr.periodic.phase = 0;
        descr.periodic.period = 71;

        descr.periodic.attack_length = 0;
        descr.periodic.attack_level = 32767; //10000
        descr.periodic.fade_length = 370;
        descr.periodic.fade_level = 0;

        updater = descr;
    }

    void Update(float power, float dur, float x,float y)
    {
        updater.periodic.magnitude = 32767.0 * power;

        updater.periodic.attack_length = 0;
        updater.periodic.attack_level = 32767.0 * power; //10000
        updater.periodic.fade_length = dur * 0.37;
        updater.periodic.fade_level = 0;

        updater.periodic.length =  dur * 0.63;

        updater.periodic.direction.dir[0] = x * 1000.0;
        updater.periodic.direction.dir[1] = y * 1000.0;

        FFEffect::UpdateHaptic(updater);
    }

protected:
    SDL_HapticEffect updater;
};

}}

#endif
