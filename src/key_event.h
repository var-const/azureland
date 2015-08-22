#include <SDL/SDL_keysym.h>
#include <bitset>

// Only those used for now, and a handful of really common keys
enum class KeyCode {
    Unknown,

    Backspace = SDLK_BACKSPACE,
    Escape = SDLK_ESCAPE,
    Return = SDLK_RETURN,
    Space = SDLK_SPACE,

    a = SDLK_a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,

    Up = SDLK_UP, Down, Right, Left,
};

// No need for meta/etc. for our purposes
enum class Modifiers { ModShift, ModAlt, ModCtrl, ModCount };
using ModifiersT = std::bitset<static_cast<size_t>(ModCount)>;

ModifiersT convert_sdl_modifiers(SDLMod const sdl_mods)
{
    ModifiersT mods;
    mods[ModShift] = (sdl_mods & KMOD_LSHIFT) | (sdl_mods & KMOD_RSHIFT);
    mods[ModAlt] = (sdl_mods & KMOD_LALT) | (sdl_mods & KMOD_RALT);
    mods[ModCtrl] = (sdl_mods & KMOD_LCTRL) | (sdl_mods & KMOD_RCTRL);
    return mods;
}

KeyCode convert_sdl_keycodes(SDLKey const sdl_key)
{
    switch (sdl_key) {
        case SDLK_BACKSPACE: return KeyCode::Backspace;
        case SDLK_RETURN: return KeyCode::Return;
        case SDLK_SPACE: return KeyCode::Space;
        case SDLK_ESCAPE: return KeyCode::Escape;
        case SDLK_UP: return KeyCode::Up;
        case SDLK_DOWN: return KeyCode::Down;
        case SDLK_LEFT: return KeyCode::Left;
        case SDLK_RIGHT: return KeyCode::Right;
        case SDLK_a: return KeyCode::a;
        case SDLK_b: return KeyCode::b;
        case SDLK_c: return KeyCode::c;
        case SDLK_d: return KeyCode::d;
        case SDLK_e: return KeyCode::e;
        case SDLK_f: return KeyCode::f;
        case SDLK_g: return KeyCode::g;
        case SDLK_h: return KeyCode::h;
        case SDLK_i: return KeyCode::i;
        case SDLK_j: return KeyCode::j;
        case SDLK_k: return KeyCode::k;
        case SDLK_l: return KeyCode::l;
        case SDLK_m: return KeyCode::m;
        case SDLK_n: return KeyCode::n;
        case SDLK_o: return KeyCode::o;
        case SDLK_p: return KeyCode::p;
        case SDLK_q: return KeyCode::q;
        case SDLK_r: return KeyCode::r;
        case SDLK_s: return KeyCode::s;
        case SDLK_t: return KeyCode::t;
        case SDLK_u: return KeyCode::u;
        case SDLK_v: return KeyCode::v;
        case SDLK_w: return KeyCode::w;
        case SDLK_x: return KeyCode::x;
        case SDLK_y: return KeyCode::y;
        case SDLK_z: return KeyCode::z;
        default:
            // @TODO: log
            return KeyCode::Unknown;
    }
    // We don't need to handle all the sdl types extensively, so we don't throw
    // any exceptions on "bad" input
}

bool is_char(int const keycode)
{
    return (keycode >= 32 && keycode <= 64) || (keycode >= 91 && keycode <= 122);
}

class KeyEvent {
public:
    KeyEvent(KeyCode const code, const ModifiersT& modifiers) : code_{code},
        modifiers_{modifiers} {}
    KeyEvent(SDL_keysym const info) : KeyEvent(convert_sdl_keycodes(info.sym),
        convert_sdl_modifiers(info.mod)) {}

    KeyCode get_code() const { return code_; }
    std::experimental::optional<char> get_char() const
        { return is_char(static_cast<int>(code_)) ? {static_cast<char>(code_)} : {}; }
    bool with_shift() const { return modifiers_[ModShift]; }
    bool with_alt() const { return modifiers_[ModAlt]; }
    bool with_ctrl() const { return modifiers_[ModCtrl]; }

private:
    KeyCode code_{KeyCode::Unknown};
    ModifiersT modifiers_;
};
