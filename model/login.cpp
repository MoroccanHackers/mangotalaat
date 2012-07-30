#include "login.h"

Login::Login(int id, User user, QDateTime eventTime, LOGIN_TYPE eventType)
{
    this->m_id = id;
    this->m_user = user;
    this->m_eventTime = eventTime;
    this->m_eventType = eventType;
}