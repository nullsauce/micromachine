//
// Created by joel on 19/04/2020.
//

#ifndef MICROMACHINE_CORE_CM0_H
#define MICROMACHINE_CORE_CM0_H


/**
 * This file integrate partially some utilities from the cmsis library.
 */

// TODO we should use cmsis library


#ifdef __cplusplus
#define __I volatile /*!< Defines 'read only' permissions */
#else
#define __I volatile const /*!< Defines 'read only' permissions */
#endif
#define __O volatile  /*!< Defines 'write only' permissions */
#define __IO volatile /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define __IM volatile const /*! Defines 'read only' structure member permissions */
#define __OM volatile       /*! Defines 'write only' structure member permissions */
#define __IOM volatile      /*! Defines 'read / write' structure member permissions */


#endif // MICROMACHINE_CORE_CM0_H
