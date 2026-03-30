package com.mycompany.mavenproject1;

import java.util.Hashtable;

public class KWTable {

	private Hashtable mTable;

	public KWTable() {
		mTable = new Hashtable();
		mTable.put("begin", new Integer(sym.BEGIN));
		mTable.put("end", new Integer(sym.END));
		mTable.put("select", new Integer(sym.SELECT));
		mTable.put("case", new Integer(sym.CASE));	}

	public int find(String keyword) {
		Object symbol = mTable.get(keyword);
		if (symbol != null)
			return ((Integer) symbol).intValue();

		return sym.ID;
	}
}