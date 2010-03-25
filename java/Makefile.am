%.class : %.java ; javac $<

CLASSES = org/odmg/NotImplementedException.class \
	org/odmg/ObjectDeletedException.class \
	org/odmg/DArray.class \
	org/odmg/ODMGRuntimeException.class \
	org/odmg/ODMGException.class \
	org/odmg/ObjectNameNotUniqueException.class \
	org/odmg/LockNotGrantedException.class \
	org/odmg/DList.class \
	org/odmg/Implementation.class \
	org/odmg/QueryParameterTypeInvalidException.class \
	org/odmg/Transaction.class \
	org/odmg/DCollection.class \
	org/odmg/TransactionAbortedException.class \
	org/odmg/DMap.class \
	org/odmg/DatabaseOpenException.class \
	org/odmg/ClassNotPersistenceCapableException.class \
	org/odmg/DSet.class \
	org/odmg/DatabaseClosedException.class \
	org/odmg/ObjectNameNotFoundException.class \
	org/odmg/OQLQuery.class \
	org/odmg/DatabaseNotFoundException.class \
	org/odmg/DBag.class \
	org/odmg/TransactionNotInProgressException.class \
	org/odmg/DatabaseIsReadOnlyException.class \
	org/odmg/QueryParameterCountInvalidException.class \
	org/odmg/TransactionInProgressException.class \
	org/odmg/Database.class \
	org/odmg/QueryException.class \
	org/odmg/ObjectNotPersistentException.class \
	org/odmg/QueryInvalidException.class \
	rasj/RasClientInternalException.class \
	rasj/RasPointType.class \
	rasj/RasType.class \
	rasj/RasTypeNotSupportedException.class \
	rasj/RasTypeUnknownException.class \
	rasj/RasTypeInvalidException.class \
	rasj/RasMArrayLong.class \
	rasj/RasMArrayDouble.class\ 
	rasj/RasMArrayInteger.class \
	rasj/RasOIDType.class \
	rasj/RasRuntimeException.class \
	rasj/RasStructureType.class \
	rasj/RasException.class \
	rasj/RasResultIsNoCellException.class \
	rasj/RasStreamInputOverflowException.class \
	rasj/RasConnectionFailedException.class \
	rasj/clientcommhttp/RasCommDefs.class \
	rasj/clientcommhttp/RasHttpRequest.class \
	rasj/clientcommhttp/RasUtils.class \
	rasj/RasMArrayShort.class \
	rasj/RasMArrayFloat.class\ 
	rasj/RasMArrayType.class \
	rasj/RasDimensionMismatchException.class \
	rasj/RasInvalidNameException.class \
	rasj/odmg/RasTransaction.class\
	rasj/odmg/RasList.class\
	rasj/odmg/RasSet.class\
	rasj/odmg/RasODMGImplementation.class\
	rasj/odmg/RasBag.class\
	rasj/odmg/RasObject.class\
	rasj/odmg/RasDatabase.class\
	rasj/odmg/RasODMGGlobal.class\
	rasj/odmg/RasOID.class\
	rasj/odmg/RasCollection.class\
	rasj/odmg/RasOQLQuery.class\
	rasj/RasStorageLayout.class\
	rasj/RasIllegalUShortValueException.class\
	rasj/RasBaseType.class\
	rasj/RasImplementationInterface.class\
	rasj/global/Version.class\
	rasj/global/RasGlobalDefs.class\
	rasj/global/BenchmarkTimer.class\
	rasj/global/RasErrorTexts.class\
	rasj/global/Debug.class\
	rasj/RasMInterval.class\
	rasj/RasMIntervalType.class\
	rasj/RasQueryExecutionFailedException.class\
	rasj/RasResultIsNoIntervalException.class\
	rasj/RasSIntervalType.class\
	rasj/RasIllegalULongValueException.class\
	rasj/RasPrimitiveType.class\
	rasj/RasIndexOutOfBoundsException.class\
	rasj/RasMArrayByte.class\
	rasj/RasCollectionType.class\
	rasj/rnp/Rnp.class\
	rasj/rnp/RnpFragmentHeader.class\
	rasj/rnp/RasRNPImplementation.class\
	rasj/rnp/RnpParameter.class\
	rasj/rnp/RnpBaseClientComm.class\
	rasj/rnp/RnpMessage.class\
	rasj/rnp/RnpDecoder.class\
	rasj/rnp/RnpMessageHeader.class\
	rasj/rnp/RnpException.class\
	rasj/rnp/RnpEncoder.class\
	rasj/rnp/RnpFragment.class\
	rasj/RasGMArray.class\
	rasj/RasPoint.class\
	rasj/RasImplementation.class\
	rasj/RasFastScale.class\
	rasj/RasSInterval.class


all : rasj

rasj : $(CLASSES)
	$(JAR_TOOL) -cf rasj.jar $(CLASSES)

CLEANFILES = $(CLASSES) rasj.jar
