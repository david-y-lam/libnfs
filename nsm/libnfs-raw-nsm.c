/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "libnfs-zdr.h"
#include "libnfs-raw-nsm.h"

bool_t
zdr_nsmstat1 (ZDR *zdrs, nsmstat1 *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_enum (zdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_nsm_my_id (ZDR *zdrs, nsm_my_id *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_string (zdrs, &objp->my_name, NSM_MAXSTRLEN))
		 return FALSE;
	 if (!zdr_int (zdrs, &objp->my_prog))
		 return FALSE;
	 if (!zdr_int (zdrs, &objp->my_vers))
		 return FALSE;
	 if (!zdr_int (zdrs, &objp->my_proc))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_nsm_mon_id (ZDR *zdrs, nsm_mon_id *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_string (zdrs, &objp->mon_name, NSM_MAXSTRLEN))
		 return FALSE;
	 if (!zdr_nsm_my_id (zdrs, &objp->my_id))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_STATres (ZDR *zdrs, NSM1_STATres *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_nsmstat1 (zdrs, &objp->res))
		 return FALSE;
	 if (!zdr_int (zdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_STATargs (ZDR *zdrs, NSM1_STATargs *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_string (zdrs, &objp->mon_name, NSM_MAXSTRLEN))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_MONres (ZDR *zdrs, NSM1_MONres *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_nsmstat1 (zdrs, &objp->res))
		 return FALSE;
	 if (!zdr_int (zdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_MONargs (ZDR *zdrs, NSM1_MONargs *objp)
{
	register int32_t *buf;
	buf = NULL;

	int i;
	 if (!zdr_nsm_mon_id (zdrs, &objp->mon_id))
		 return FALSE;
	 if (!zdr_opaque (zdrs, objp->priv, 16))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_UNMONres (ZDR *zdrs, NSM1_UNMONres *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_int (zdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_UNMONargs (ZDR *zdrs, NSM1_UNMONargs *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_nsm_mon_id (zdrs, &objp->mon_id))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_UNMONALLres (ZDR *zdrs, NSM1_UNMONALLres *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_int (zdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_UNMONALLargs (ZDR *zdrs, NSM1_UNMONALLargs *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_nsm_my_id (zdrs, &objp->my_id))
		 return FALSE;
	return TRUE;
}

bool_t
zdr_NSM1_NOTIFYargs (ZDR *zdrs, NSM1_NOTIFYargs *objp)
{
	register int32_t *buf;
	buf = NULL;

	 if (!zdr_string (zdrs, &objp->mon_name, NSM_MAXSTRLEN))
		 return FALSE;
	 if (!zdr_int (zdrs, &objp->state))
		 return FALSE;
	return TRUE;
}
