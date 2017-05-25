#!/usr/bin/env python3
# Copyright 2017 H2O.ai; Apache License Version 2.0;  -*- encoding: utf-8 -*-
import pytest
import datatable as dt


#-------------------------------------------------------------------------------
# Prepare fixtures & helper functions
#-------------------------------------------------------------------------------

@pytest.fixture()
def dt0():
    return dt.DataTable({
        "A": [2, 7, 0, 0],
        "B": [True, False, False, True],
        "C": [1, 1, 1, 1],
        "D": [0.1, 2, -4, 4.4],
        "E": [None, None, None, None],
        "F": [0, 0, 0, 0],
        "G": [1, 2, "hello", "world"],
    })

def assert_valueerror(datatable, rows, error_message):
    with pytest.raises(ValueError) as e:
        datatable(rows=rows)
    assert str(e.type) == "<class 'dt.ValueError'>"
    assert error_message in str(e.value)



#-------------------------------------------------------------------------------
# Run the tests
#-------------------------------------------------------------------------------

def test_dt_properties(dt0):
    assert isinstance(dt0, dt.DataTable)
    assert dt0.nrows == 4
    assert dt0.ncols == 7
    assert dt0.shape == (4, 7)
    assert dt0.names == ("A", "B", "C", "D", "E", "F", "G")
    assert dt0.types == ("int", "bool", "bool", "real", "bool", "bool", "str")
    assert dt0.stypes == ("i1i", "i1b", "i1b", "f8r", "i1b", "i1b", "i4s")
    assert dt0.internal.verify_integrity() is None


def test_dt_call(dt0, capsys):
    dt1 = dt0(timeit=True)
    assert dt1.shape == dt0.shape
    assert dt1.internal.isview and dt1.internal.rowmapping_type == "slice"
    out, err = capsys.readouterr()
    assert err == ""
    assert "Time taken:" in out


def test_dt_view(dt0, monkeypatch, capsys):
    def send1(refresh_rate=1):
        yield None
    monkeypatch.setattr("datatable.utils.terminal.wait_for_keypresses", send1)
    dt0.view()
    out, err = capsys.readouterr()
    assert ("     A   B   C     D   E   F  G    \n"
            "--  --  --  --  ----  --  --  -----\n"
            " 0   2   1   1   0.1       0  1    \n"
            " 1   7   0   1   2         0  2    \n"
            " 2   0   0   1  -4         0  hello\n"
            " 3   0   1   1   4.4       0  world\n"
            "\n"
            "[4 rows x 7 columns]\n"
            "Press q to quit  ↑←↓→ to move  wasd to page  t to toggle types"
            in out)


def test_dt_colindex(dt0):
    for i, ch in enumerate("ABCDEFG"):
        assert dt0.colindex(ch) == i
    with pytest.raises(ValueError) as e:
        dt0.colindex("a")
    assert "Column 'a' does not exist" in str(e.value)


def test_dt_getitem(dt0):
    dt1 = dt0[0]
    assert dt1.shape == (4, 1)
    assert dt1.names == ("A", )
    dt1 = dt0[(4,)]
    assert dt1.shape == (4, 1)
    assert dt1.names == ("E", )
    dt2 = dt0[0, 1]
    assert dt2.shape == (1, 1)
    assert dt2.names == ("B", )
    with pytest.raises(ValueError) as e:
        dt0[0, 1, 2, 3]
    assert "Selector (0, 1, 2, 3) is not supported" in str(e.value)


def test__hex(dt0, monkeypatch, capsys):
    def send1(refresh_rate=1):
        yield None
    monkeypatch.setattr("datatable.utils.terminal.wait_for_keypresses", send1)
    dt.utils.terminal.term.override_width = 100

    dt0._hex(-1)
    out, err = capsys.readouterr()
    print(out)
    assert ("Column 6, Name: 'G'\n"
            "Ltype: str, Stype: i4s, Mtype: data\n"
            "Data size: 32\n"
            "Meta: offoff=16\n"
            "    00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F                  \n"
            "--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  ----------------\n"
            " 0  31  32  68  65  6C  6C  6F  77  6F  72  6C  64  FF  FF  FF  FF  12helloworldÿÿÿÿ\n"
            " 1  02  00  00  00  03  00  00  00  08  00  00  00  0D  00  00  00  ................\n"
            in out)

    with pytest.raises(ValueError) as e:
        dt0._hex(10)
    assert "Invalid column index" in str(e.value)

    dt0[::2]._hex(1)
    out, err = capsys.readouterr()
    assert ("Column 1, Name: 'C'\n"
            "Ltype: bool, Stype: i1b, Mtype: view\n"
            "Column index in the source datatable: 2\n"
            in out)


def test_constructor():
    d0 = dt.DataTable([1, 2, 3])
    assert d0.shape == (3, 1)
    assert d0.names == ("C1", )
    assert d0.types == ("int", )
    d1 = dt.DataTable([[1, 2], [True, False], [.3, -0]], colnames="ABC")
    assert d1.shape == (2, 3)
    assert d1.names == ("A", "B", "C")
    assert d1.types == ("int", "bool", "real")
    d2 = dt.DataTable((3, 5, 6, 0))
    assert d2.shape == (4, 1)
    assert d2.types == ("int", )
    d3 = dt.DataTable({1, 13, 15, -16, -10, 7, 9, 1})
    assert d3.shape == (7, 1)
    assert d3.types == ("int", )
    d4 = dt.DataTable()
    assert d4.shape == (0, 0)
    assert d4.names == tuple()
    assert d4.types == tuple()
    assert d4.stypes == tuple()
    d5 = dt.DataTable([])
    assert d5.shape == (0, 0)
    assert d5.names == tuple()
    assert d5.types == tuple()
    assert d5.stypes == tuple()
    d6 = dt.DataTable([[]])
    assert d6.shape == (0, 1)
    assert d6.names == ("C1", )
    assert d6.types == ("bool", )

    with pytest.raises(TypeError) as e:
        dt.DataTable("scratch")
    assert "Cannot create DataTable from 'scratch'" in str(e.value)