#include "common.hpp"
#include "Sequence/BaseRacePage.hpp"

HOOK void initTimeInGP(Sequence::BaseRacePage* baseRacePage) {
    // baseRacePage is already in r0 when the game reaches this function,
    // therefore we don't have to move registers here
    baseRacePage->initTime();
}
