#include "../play_strategies.h"

Player *play_standard_wo_sd(Player *p, Table *t){
    Deck *d;
    Hand* cHand;
    int *handValues, *cHandValues;
    if(!p || !t){
        fprintf(stderr, "play_standard_wo_sd: invalid arguments.\n");
        return NULL;
    }
    /* Get Deck from table */
    d = table_getDeck(t);

    /*Suponiendo que el jugador ya tiene las dos primeras cartas*/

    /*Suponiendo que el jugador solo tiene una mano*/

    handValues = hand_getValues(player_getHand(p, 0));
    if (!handValues) {
        fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
        return NULL;
    }

    /* Pedir carta hasta 9 */
    while (handValues[0] <= 9) {
        /* Pedir carta */
        p = player_addCardToHand(p,0, deck_draw(d));
        free(handValues);
        handValues = hand_getValues(player_getHand(p, 0));
        if (!handValues) {
            fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
            free(handValues);
            return NULL;
        }
    }
    /* 10-11 pide carta contra 10-11*/
    while (handValues[0]==10 || handValues[0]==11){
        cHand=crupier_getHand(table_getCrupier(t));
        if(!cHand){
            fprintf(stderr, "play_standard_wo_sd: error getting crupier hand.\n");
            return NULL;
        }
        cHandValues=hand_getValues(cHand);
        if(cHandValues[0]!=10 && cHandValues[0]!=11){
            /* Pedir carta */
            p = player_addCardToHand(p,0, deck_draw(d));
            free(handValues);
            handValues = hand_getValues(player_getHand(p, 0));
            if (!handValues) {
                fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                free(handValues);
                return NULL;
            }
        }
        else{
            p = player_addCardToHand(p,0, deck_draw(d));
            free(handValues);
            handValues = hand_getValues(player_getHand(p, 0));
            if (!handValues) {
                fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                free(handValues);
                return NULL;
            }
        }
    }
    /* Se planta con 12 si hay 4, 5 o 6*/
    if (handValues[0]==12){
        if (cHandValues[0]==4 || cHandValues[0]==5 || cHandValues[0]==6){
            /* Se planta */
            return p;
        }
        else {
            /* Pide carta*/
            p = player_addCardToHand(p,0, deck_draw(d));
            free(handValues);
            handValues = hand_getValues(player_getHand(p, 0));
        }
    }
    /* Con 13, 14, 15 o 16 se planta contra 2,3,4,5,6 pide en otro caso*/
    while(handValues[0]<17 && handValues[0]>12){
        if(cHandValues[0]<7){
            /* Se planta*/
            return p;
        }
        else{
            /* Pide carta */
            p = player_addCardToHand(p,0, deck_draw(d));
            free(handValues);
            handValues = hand_getValues(player_getHand(p, 0));
            if (!handValues) {
                fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                free(handValues);
                return NULL;
            }
        }
    }
    /* Si la mano es 8-18, pide contra 9,10,11 */
    if (handValues[0]==18 && handValues[1]==8){
        if(cHandValues[0]>8 && cHandValues[0]<12){
            /*Pide carta*/
            p = player_addCardToHand(p,0, deck_draw(d));
            free(handValues);
            handValues = hand_getValues(player_getHand(p, 0));
            if (!handValues) {
                fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                free(handValues);
                return NULL;
            }
        }
        else {
            /*Se planta*/
            return p;
        }
    }
    /* Si la mano es 9-19 se planta */
    if (handValues[0]==19 && handValues[1]==9){
        /*Se planta*/
        return p;
    }
    /* Si la mano es 10-20 se planta */
    if (handValues[0]==20 && handValues[1]==10){
        /*Se planta*/
        return p;
    }
    /* Si la mano es 11-21 se planta */
    if (handValues[0]==21 && handValues[1]==11){
        /*Se planta*/
        return p;
    }
    if (handValues[0]>=17 || handValues[0]<=21){
        /*Se planta*/
        return p;
    }
    if (handValues[1]>0){
        /* Pedir carta hasta 9 */
        while (handValues[1] <= 9) {
            /* Pedir carta */
            p = player_addCardToHand(p,0, deck_draw(d));
            free(handValues);
            handValues = hand_getValues(player_getHand(p, 0));
            if (!handValues) {
                fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                free(handValues);
                return NULL;
            }
        }
        /* 10-11 pide carta contra 10-11*/
        while (handValues[1]==10 || handValues[1]==11){
            cHand=crupier_getHand(table_getCrupier(t));
            if(!cHand){
                fprintf(stderr, "play_standard_wo_sd: error getting crupier hand.\n");
                return NULL;
            }
            cHandValues=hand_getValues(cHand);
            if(cHandValues[0]!=10 && cHandValues[0]!=11){
                /* Pedir carta */
                p = player_addCardToHand(p,0, deck_draw(d));
                free(handValues);
                handValues = hand_getValues(player_getHand(p, 0));
                if (!handValues) {
                    fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                    free(handValues);
                    return NULL;
                }
            }
            else{
                /* Pedir carta */
                p = player_addCardToHand(p,0, deck_draw(d));
                free(handValues);
                handValues = hand_getValues(player_getHand(p, 0));
                if (!handValues) {
                    fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                    free(handValues);
                    return NULL;
                }
            }
        }
        /* Se planta con 12 si hay 4, 5 o 6*/
        if (handValues[1]==12){
            if (cHandValues[0]==4 || cHandValues[0]==5 || cHandValues[0]==6){
                /* Se planta */
                return p;
            }
            else {
                /* Pide carta*/
                p = player_addCardToHand(p,0, deck_draw(d));
                free(handValues);
                handValues = hand_getValues(player_getHand(p, 0));
            }
        }
        /* Con 13, 14, 15 o 16 se planta contra 2,3,4,5,6 pide en otro caso*/
        while(handValues[1]<17 && handValues[1]>12){
            if(cHandValues[0]<7){
                /* Se planta*/
                return p;
            }
            else{
                /* Pide carta */
                p = player_addCardToHand(p,0, deck_draw(d));
                free(handValues);
                handValues = hand_getValues(player_getHand(p, 0));
                if (!handValues) {
                    fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                    free(handValues);
                    return NULL;
                }
            }
        }
        if (handValues[1]>=17){
            /*Se planta*/
            return p;
        }
    }
    return NULL;
  }
