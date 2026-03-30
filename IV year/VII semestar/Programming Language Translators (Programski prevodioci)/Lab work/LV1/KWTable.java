package com.mycompany.mavenproject1;

import java.util.Hashtable;

public class KWTable {

	private Hashtable mTable;

	public KWTable() {
		mTable = new Hashtable();
		mTable.put("program", new Integer(sym.PROGRAM));
		mTable.put("begin", new Integer(sym.BEGIN));
		mTable.put("end", new Integer(sym.END));
		mTable.put("select", new Integer(sym.SELECT));
		mTable.put("case", new Integer(sym.CASE));
		mTable.put("or", new Integer(sym.OR));
		mTable.put("and", new Integer(sym.AND));
		mTable.put("integer", new Integer(sym.INTEGER));
		mTable.put("char", new Integer(sym.CHAR));
		mTable.put("real", new Integer(sym.REAL));
		mTable.put("boolean", new Integer(sym.BOOLEAN));
	}

	public int find(String keyword) {
		Object symbol = mTable.get(keyword);
		if (symbol != null)
			return ((Integer) symbol).intValue();

		return sym.ID;
	}
}