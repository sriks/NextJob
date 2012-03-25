#ifndef CONSTANTS_H
#define CONSTANTS_H

/*!
  Constants used across nextjob.
  **/

#include <QString>

const QString APP_FOLDER_NAME("nj");

/* Request parameters */
const QString NJ_SKILL("skill");
const QString NJ_LOCATION("location");
const QString NJ_COUNTRY("country");
/* Request parameters */
// used internally to identify the favorites file path.
const QString NJ_FAV_PATH("favspath");

/* JobInfo keys */
//const QString NJ_PROP_KEY_ISVALID("isvalid");
const QString NJ_PROP_KEY_TITLE("title");
const QString NJ_PROP_KEY_DESCRIPTION("description");
const QString NJ_PROP_KEY_DETAILED_DESCRIPTION_FETCHED("det_desc_fetched");
const QString NJ_PROP_KEY_EMPNAME("empname");
const QString NJ_PROP_KEY_LOCATION("location");
const QString NJ_PROP_KEY_URL("link");
const QString NJ_PROP_KEY_SOURCE("source");
const QString NJ_PROP_KEY_DATE("pubDate");

/* JobInfo keys */

#endif // CONSTANTS_H
