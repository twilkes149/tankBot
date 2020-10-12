#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include <windows.h>

namespace client {

  class Keyboard {
    public:
      typedef enum {
        A_KEY = 'A',
        B_KEY,
        C_KEY,
        D_KEY,
        E_KEY,
        F_KEY,
        G_KEY,
        H_KEY,
        I_KEY,
        J_KEY,
        K_KEY,
        L_KEY,
        M_KEY,
        N_KEY,
        O_KEY,
        P_KEY,
        Q_KEY,
        R_KEY,
        S_KEY,
        T_KEY,
        U_KEY,
        V_KEY,
        W_KEY,
        X_KEY,
        Y_KEY,
        Z_KEY
      } key_t;

      const int PRESSED_STATE;
      static const int NUM_KEYS = 26;

    public:
      Keyboard();

      /*
      * \brief returns true if the given key is pressed
      */
      bool isPressed(const key_t key);

      /*
      * \brief - hashes the value of a key to an array index, where A_KEY returns 0
      */
      int keyToIndexHash(const key_t key);
  };
}

#endif