#include "mainInclude.hpp"
#include "adm.hpp"

std::map<int, Rights> AdminList;

void Adm::SetEnvViaMod(rightModes *m, Rights *r){
   switch (static_cast<int>(*m))
   {
   case  0:{
     r->ADMIN_LIST_EDITING = true;
     r->CRYPT_DATA_EDITING = true;
     r->CRYPT_EDITING = true;
     r->DATA_EDITING = true;
     r->SIMULATION = true;
     break;
   }
   case 3:{
    r->ADMIN_LIST_EDITING = false;
    r->CRYPT_DATA_EDITING = true;
    r->CRYPT_EDITING = false;
    r->DATA_EDITING = false;
    r->SIMULATION = false;
    break;
   }
   case 1:{
    r->ADMIN_LIST_EDITING = false;
    r->CRYPT_DATA_EDITING = false;
    r->DATA_EDITING = true;
    r->CRYPT_EDITING = false;
    r->SIMULATION = false;
    break;
   }
   case 2:{
    r->ADMIN_LIST_EDITING = false;
    r->CRYPT_DATA_EDITING = false;
    r->CRYPT_EDITING = true;
    r->SIMULATION = false;
    r->DATA_EDITING = false;
   }
   case 4:
    break;
   }
}

void Adm::SaveAdmin(Rights *r){
  AdminList[r->tgId] = *r;
  debug("inf", "admin(" + std::to_string(r->tgId) + ") saved");
}