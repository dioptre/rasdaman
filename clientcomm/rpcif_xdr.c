/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "rpcif.h"

bool_t
xdr_confarray (XDR *xdrs, confarray *objp)
{
	register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->confarray_val, (u_int *) &objp->confarray_len, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RPCMarray (XDR *xdrs, RPCMarray *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_string (xdrs, &objp->domain, ~0))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->cellTypeLength))
				 return FALSE;
			 if (!xdr_u_short (xdrs, &objp->currentFormat))
				 return FALSE;
			 if (!xdr_u_short (xdrs, &objp->storageFormat))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->cellTypeLength);
		IXDR_PUT_U_SHORT(buf, objp->currentFormat);
		IXDR_PUT_U_SHORT(buf, objp->storageFormat);
		}
		 if (!xdr_confarray (xdrs, &objp->data))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_string (xdrs, &objp->domain, ~0))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->cellTypeLength))
				 return FALSE;
			 if (!xdr_u_short (xdrs, &objp->currentFormat))
				 return FALSE;
			 if (!xdr_u_short (xdrs, &objp->storageFormat))
				 return FALSE;

		} else {
		objp->cellTypeLength = IXDR_GET_U_LONG(buf);
		objp->currentFormat = IXDR_GET_U_SHORT(buf);
		objp->storageFormat = IXDR_GET_U_SHORT(buf);
		}
		 if (!xdr_confarray (xdrs, &objp->data))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_string (xdrs, &objp->domain, ~0))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->cellTypeLength))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->currentFormat))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->storageFormat))
		 return FALSE;
	 if (!xdr_confarray (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RPCClientEntry (XDR *xdrs, RPCClientEntry *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_u_long (xdrs, &objp->clientId))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->clientIdText, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->userName, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->baseName, ~0))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 9 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->creationTime))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->lastActionTime))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transferColl))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transferIter))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->assembleMDD))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transferMDD))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transTiles))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->tileIter))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->bytesToTransfer))
				 return FALSE;
		} else {
			IXDR_PUT_U_LONG(buf, objp->creationTime);
			IXDR_PUT_U_LONG(buf, objp->lastActionTime);
			IXDR_PUT_U_LONG(buf, objp->transferColl);
			IXDR_PUT_U_LONG(buf, objp->transferIter);
			IXDR_PUT_U_LONG(buf, objp->assembleMDD);
			IXDR_PUT_U_LONG(buf, objp->transferMDD);
			IXDR_PUT_U_LONG(buf, objp->transTiles);
			IXDR_PUT_U_LONG(buf, objp->tileIter);
			IXDR_PUT_U_LONG(buf, objp->bytesToTransfer);
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_u_long (xdrs, &objp->clientId))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->clientIdText, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->userName, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->baseName, ~0))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 9 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->creationTime))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->lastActionTime))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transferColl))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transferIter))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->assembleMDD))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transferMDD))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transTiles))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->tileIter))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->bytesToTransfer))
				 return FALSE;
		} else {
			objp->creationTime = IXDR_GET_U_LONG(buf);
			objp->lastActionTime = IXDR_GET_U_LONG(buf);
			objp->transferColl = IXDR_GET_U_LONG(buf);
			objp->transferIter = IXDR_GET_U_LONG(buf);
			objp->assembleMDD = IXDR_GET_U_LONG(buf);
			objp->transferMDD = IXDR_GET_U_LONG(buf);
			objp->transTiles = IXDR_GET_U_LONG(buf);
			objp->tileIter = IXDR_GET_U_LONG(buf);
			objp->bytesToTransfer = IXDR_GET_U_LONG(buf);
		}
	 return TRUE;
	}

	 if (!xdr_u_long (xdrs, &objp->clientId))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->clientIdText, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->userName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->baseName, ~0))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->creationTime))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->lastActionTime))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->transferColl))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->transferIter))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->assembleMDD))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->transferMDD))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->transTiles))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->tileIter))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->bytesToTransfer))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RPCOIdEntry (XDR *xdrs, RPCOIdEntry *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_OpenDBParams (XDR *xdrs, OpenDBParams *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->dbName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->userName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->capability, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_OpenDBRes (XDR *xdrs, OpenDBRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_BeginTAParams (XDR *xdrs, BeginTAParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->readOnly))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->capability, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ExecuteQueryParams (XDR *xdrs, ExecuteQueryParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->query, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ExecuteQueryRes (XDR *xdrs, ExecuteQueryRes *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_short (xdrs, &objp->status))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->errorNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->lineNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->columnNo))
				 return FALSE;

		} else {
		IXDR_PUT_U_SHORT(buf, objp->status);
		IXDR_PUT_U_LONG(buf, objp->errorNo);
		IXDR_PUT_U_LONG(buf, objp->lineNo);
		IXDR_PUT_U_LONG(buf, objp->columnNo);
		}
		 if (!xdr_string (xdrs, &objp->token, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->typeName, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_short (xdrs, &objp->status))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->errorNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->lineNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->columnNo))
				 return FALSE;

		} else {
		objp->status = IXDR_GET_U_SHORT(buf);
		objp->errorNo = IXDR_GET_U_LONG(buf);
		objp->lineNo = IXDR_GET_U_LONG(buf);
		objp->columnNo = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_string (xdrs, &objp->token, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->typeName, ~0))
			 return FALSE;
		 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->errorNo))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->lineNo))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->columnNo))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->token, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ExecuteUpdateRes (XDR *xdrs, ExecuteUpdateRes *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_short (xdrs, &objp->status))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->errorNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->lineNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->columnNo))
				 return FALSE;

		} else {
		IXDR_PUT_U_SHORT(buf, objp->status);
		IXDR_PUT_U_LONG(buf, objp->errorNo);
		IXDR_PUT_U_LONG(buf, objp->lineNo);
		IXDR_PUT_U_LONG(buf, objp->columnNo);
		}
		 if (!xdr_string (xdrs, &objp->token, ~0))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_short (xdrs, &objp->status))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->errorNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->lineNo))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->columnNo))
				 return FALSE;

		} else {
		objp->status = IXDR_GET_U_SHORT(buf);
		objp->errorNo = IXDR_GET_U_LONG(buf);
		objp->lineNo = IXDR_GET_U_LONG(buf);
		objp->columnNo = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_string (xdrs, &objp->token, ~0))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->errorNo))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->lineNo))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->columnNo))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->token, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_InsertCollParams (XDR *xdrs, InsertCollParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_NameSpecParams (XDR *xdrs, NameSpecParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->name, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_OIdSpecParams (XDR *xdrs, OIdSpecParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RemoveObjFromCollParams (XDR *xdrs, RemoveObjFromCollParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetCollRes (XDR *xdrs, GetCollRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetCollOIdsRes (XDR *xdrs, GetCollOIdsRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->oidTable.oidTable_val, (u_int *) &objp->oidTable.oidTable_len, ~0,
		sizeof (RPCOIdEntry), (xdrproc_t) xdr_RPCOIdEntry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetMDDRes (XDR *xdrs, GetMDDRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->domain, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->currentFormat))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetTileRes (XDR *xdrs, GetTileRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->marray, sizeof (RPCMarray), (xdrproc_t) xdr_RPCMarray))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_OIdRes (XDR *xdrs, OIdRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ObjectTypeRes (XDR *xdrs, ObjectTypeRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->objType))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_InsertMDDParams (XDR *xdrs, InsertMDDParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->marray, sizeof (RPCMarray), (xdrproc_t) xdr_RPCMarray))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_InsertTileParams (XDR *xdrs, InsertTileParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->isPersistent))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->marray, sizeof (RPCMarray), (xdrproc_t) xdr_RPCMarray))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_EndInsertMDDParams (XDR *xdrs, EndInsertMDDParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->isPersistent))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_InsertTransMDDParams (XDR *xdrs, InsertTransMDDParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->domain, ~0))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->typeLength))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_InsertPersMDDParams (XDR *xdrs, InsertPersMDDParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->collName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->domain, ~0))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->typeLength))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->oid, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_NewOIdParams (XDR *xdrs, NewOIdParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->objType))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ServerStatRes (XDR *xdrs, ServerStatRes *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 12 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_short (xdrs, &objp->status))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->inactivityTimeout))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->managementInterval))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transactionActive))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->maxTransferBufferSize))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->nextClientId))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->clientNumber))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memArena))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memSmblks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memOrdblks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memFordblks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memUordblks))
				 return FALSE;

		} else {
		IXDR_PUT_U_SHORT(buf, objp->status);
		IXDR_PUT_U_LONG(buf, objp->inactivityTimeout);
		IXDR_PUT_U_LONG(buf, objp->managementInterval);
		IXDR_PUT_U_LONG(buf, objp->transactionActive);
		IXDR_PUT_U_LONG(buf, objp->maxTransferBufferSize);
		IXDR_PUT_U_LONG(buf, objp->nextClientId);
		IXDR_PUT_U_LONG(buf, objp->clientNumber);
		IXDR_PUT_U_LONG(buf, objp->memArena);
		IXDR_PUT_U_LONG(buf, objp->memSmblks);
		IXDR_PUT_U_LONG(buf, objp->memOrdblks);
		IXDR_PUT_U_LONG(buf, objp->memFordblks);
		IXDR_PUT_U_LONG(buf, objp->memUordblks);
		}
		 if (!xdr_array (xdrs, (char **)&objp->clientTable.clientTable_val, (u_int *) &objp->clientTable.clientTable_len, ~0,
			sizeof (RPCClientEntry), (xdrproc_t) xdr_RPCClientEntry))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 12 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_short (xdrs, &objp->status))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->inactivityTimeout))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->managementInterval))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->transactionActive))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->maxTransferBufferSize))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->nextClientId))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->clientNumber))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memArena))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memSmblks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memOrdblks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memFordblks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->memUordblks))
				 return FALSE;

		} else {
		objp->status = IXDR_GET_U_SHORT(buf);
		objp->inactivityTimeout = IXDR_GET_U_LONG(buf);
		objp->managementInterval = IXDR_GET_U_LONG(buf);
		objp->transactionActive = IXDR_GET_U_LONG(buf);
		objp->maxTransferBufferSize = IXDR_GET_U_LONG(buf);
		objp->nextClientId = IXDR_GET_U_LONG(buf);
		objp->clientNumber = IXDR_GET_U_LONG(buf);
		objp->memArena = IXDR_GET_U_LONG(buf);
		objp->memSmblks = IXDR_GET_U_LONG(buf);
		objp->memOrdblks = IXDR_GET_U_LONG(buf);
		objp->memFordblks = IXDR_GET_U_LONG(buf);
		objp->memUordblks = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_array (xdrs, (char **)&objp->clientTable.clientTable_val, (u_int *) &objp->clientTable.clientTable_len, ~0,
			sizeof (RPCClientEntry), (xdrproc_t) xdr_RPCClientEntry))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->inactivityTimeout))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->managementInterval))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->transactionActive))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->maxTransferBufferSize))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->nextClientId))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->clientNumber))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->memArena))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->memSmblks))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->memOrdblks))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->memFordblks))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->memUordblks))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->clientTable.clientTable_val, (u_int *) &objp->clientTable.clientTable_len, ~0,
		sizeof (RPCClientEntry), (xdrproc_t) xdr_RPCClientEntry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ServerVersionRes (XDR *xdrs, ServerVersionRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->serverVersionNo))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->rpcInterfaceVersionNo))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetTypeStructureParams (XDR *xdrs, GetTypeStructureParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeName, ~0))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->typeType))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetTypeStructureRes (XDR *xdrs, GetTypeStructureRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->typeStructure, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetElementRes (XDR *xdrs, GetElementRes *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_confarray (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_SetServerTransferParams (XDR *xdrs, SetServerTransferParams *objp)
{
	register int32_t *buf;

	 if (!xdr_u_long (xdrs, &objp->clientID))
		 return FALSE;
	 if (!xdr_u_short (xdrs, &objp->format))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->formatParams, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GetExtendedErrorInfo (XDR *xdrs, GetExtendedErrorInfo *objp)
{
	register int32_t *buf;

	 if (!xdr_u_short (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->errorText, ~0))
		 return FALSE;
	return TRUE;
}