package assign05;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class ArrayListSorterTest {
	
	private ArrayList<Integer> array1, singleArray;
	
	private ArrayList<String> stringArr;
	
	private ArrayList<Double> doubleArr;

	@BeforeEach
	void setUp() throws Exception {
		array1 = new ArrayList<Integer>();
		array1.add(6);
		array1.add(3);
		array1.add(10);
		array1.add(15);
		array1.add(2);
		
		singleArray = new ArrayList<Integer>();
		singleArray.add(4);
		
		stringArr = new ArrayList<String>();
		stringArr.add("Alpha");
		stringArr.add("Charlie");
		stringArr.add("Zeta");
		stringArr.add("Beta");
		stringArr.add("Hotel");
		
		doubleArr = new ArrayList<Double>();
		doubleArr.add(1.0);
		doubleArr.add(5.2);
		doubleArr.add(4.0);
		doubleArr.add(2.0);
		doubleArr.add(3.5);
	}

	//MERGESORT TESTS
	@Test
	void testMergesort() {
		ArrayListSorter.mergesort(array1);
		assertEquals(5, array1.size());
		assertEquals(2, array1.get(0));
		assertEquals(3, array1.get(1));
		assertEquals(6, array1.get(2));
		assertEquals(10, array1.get(3));
		assertEquals(15, array1.get(4));
	}
	
	@Test
	void testMergeSortLarge() {
		ArrayList<Integer> arr = ArrayListSorter.generatePermuted(50);
		ArrayListSorter.mergesort(arr);
		assertEquals(50, arr.size());
		for(int i = 0; i < 50; ++i) {
			assertEquals(i + 1, arr.get(i));
		}
	}
	
	@Test
	void testMergeSortNegativeIntegers() {
		array1.add(3, new Integer(-5));
		ArrayListSorter.mergesort(array1);
		assertEquals(6, array1.size());
		assertEquals(-5, array1.get(0));
		assertEquals(2, array1.get(1));
		assertEquals(3, array1.get(2));
		assertEquals(6, array1.get(3));
		assertEquals(10, array1.get(4));
		assertEquals(15, array1.get(5));
		
	}
	
	@Test
	void testMergeSortSingleItem() {
		ArrayListSorter.mergesort(singleArray);
		assertEquals(1, singleArray.size());
		assertEquals(4, singleArray.get(0));
	}
	
	@Test
	void testMergeSortEmpty() {
		ArrayList<Integer> emptyArr = new ArrayList<Integer>();
		ArrayListSorter.mergesort(emptyArr);
		assertEquals(0, emptyArr.size());
	}
	
	@Test
	void testMergeSortStringArray() {
		ArrayListSorter.mergesort(stringArr);
		assertEquals(5, stringArr.size());
		assertEquals("Alpha", stringArr.get(0));
		assertEquals("Beta", stringArr.get(1));
		assertEquals("Charlie", stringArr.get(2));
		assertEquals("Hotel", stringArr.get(3));
		assertEquals("Zeta", stringArr.get(4));
	}
	
	@Test
	void testMergeSortDoubleArray() {
		ArrayListSorter.mergesort(doubleArr);
		assertEquals(5, doubleArr.size());
		assertEquals(1.0, doubleArr.get(0));
		assertEquals(2.0, doubleArr.get(1));
		assertEquals(3.5, doubleArr.get(2));
		assertEquals(4.0, doubleArr.get(3));
		assertEquals(5.2, doubleArr.get(4));
	}
	
	@Test
	void testMergeSortPermutedList() {
		ArrayList<Integer> permutedArr = ArrayListSorter.generatePermuted(25);
		ArrayListSorter.mergesort(permutedArr);
		assertEquals(25, permutedArr.size());
		for(int i = 0; i < 25; ++i) {
			assertEquals(i + 1, permutedArr.get(i));
		}
	}
	
	@Test
	void testMergeSortDescendingList() {
		ArrayList<Integer> descendingArr = ArrayListSorter.generateDescending(25);
		ArrayListSorter.mergesort(descendingArr);
		assertEquals(25, descendingArr.size());
		for(int i = 0; i < 25; ++i) {
			assertEquals(i + 1, descendingArr.get(i));
		}
	}
	
	@Test
	void testMergeSortAscendingList() {
		ArrayList<Integer> ascendingArr = ArrayListSorter.generateAscending(25);
		ArrayListSorter.mergesort(ascendingArr);
		assertEquals(25, ascendingArr.size());
		for(int i = 0; i < 25; ++i) {
			assertEquals(i + 1, ascendingArr.get(i));
		}
	}
	
	@Test
	void testMergeSortDuplicateList() {
		ArrayList<Integer> arr = new ArrayList<Integer>();
		arr.add(3);
		arr.add(3);
		arr.add(1);
		arr.add(6);
		arr.add(1);
		ArrayListSorter.mergesort(arr);
		assertEquals(5, arr.size());
		assertEquals(1, arr.get(0));
		assertEquals(1, arr.get(1));
		assertEquals(3, arr.get(2));
		assertEquals(3, arr.get(3));
		assertEquals(6, arr.get(4));
	}
	//QUICKSORT TESTS
	@Test
	void testQuicksort() {
		ArrayListSorter.quicksort(array1);
		assertEquals(5, array1.size());
		assertEquals(2, array1.get(0));
		assertEquals(3, array1.get(1));
		assertEquals(6, array1.get(2));
		assertEquals(10, array1.get(3));
		assertEquals(15, array1.get(4));
	}
	
	@Test
	void testQuickSortLarge() {
		ArrayList<Integer> arr = ArrayListSorter.generatePermuted(50);
		ArrayListSorter.quicksort(arr);
		assertEquals(50, arr.size());
		for(int i = 0; i < 50; ++i) {
			assertEquals(i + 1, arr.get(i));
		}
	}
	
	@Test
	void testQuickSortNegativeIntegers() {
		array1.add(3, new Integer(-5));
		ArrayListSorter.quicksort(array1);
		assertEquals(6, array1.size());
		assertEquals(-5, array1.get(0));
		assertEquals(2, array1.get(1));
		assertEquals(3, array1.get(2));
		assertEquals(6, array1.get(3));
		assertEquals(10, array1.get(4));
		assertEquals(15, array1.get(5));
		
	}
	
	@Test
	void testQuickSortSingleItem() {
		ArrayListSorter.quicksort(singleArray);
		assertEquals(1, singleArray.size());
		assertEquals(4, singleArray.get(0));
	}
	
	@Test
	void testQuickSortEmpty() {
		ArrayList<Integer> emptyArr = new ArrayList<Integer>();
		ArrayListSorter.quicksort(emptyArr);
		assertEquals(0, emptyArr.size());
	}
	
	@Test
	void testQuickSortStringArray() {
		ArrayListSorter.quicksort(stringArr);
		assertEquals(5, stringArr.size());
		assertEquals("Alpha", stringArr.get(0));
		assertEquals("Beta", stringArr.get(1));
		assertEquals("Charlie", stringArr.get(2));
		assertEquals("Hotel", stringArr.get(3));
		assertEquals("Zeta", stringArr.get(4));
	}
	
	@Test
	void testQuickSortDoubleArray() {
		ArrayListSorter.quicksort(doubleArr);
		assertEquals(5, doubleArr.size());
		assertEquals(1.0, doubleArr.get(0));
		assertEquals(2.0, doubleArr.get(1));
		assertEquals(3.5, doubleArr.get(2));
		assertEquals(4.0, doubleArr.get(3));
		assertEquals(5.2, doubleArr.get(4));
	}
	
	@Test
	void testQuickSortPermutedList() {
		ArrayList<Integer> permutedArr = ArrayListSorter.generatePermuted(25);
		ArrayListSorter.quicksort(permutedArr);
		assertEquals(25, permutedArr.size());
		for(int i = 0; i < 25; ++i) {
			assertEquals(i + 1, permutedArr.get(i));
		}
	}
	
	@Test
	void testQuickSortDescendingList() {
		ArrayList<Integer> descendingArr = ArrayListSorter.generateDescending(25);
		ArrayListSorter.quicksort(descendingArr);
		assertEquals(25, descendingArr.size());
		for(int i = 0; i < 25; ++i) {
			assertEquals(i + 1, descendingArr.get(i));
		}
	}
	
	@Test
	void testQuickSortAscendingList() {
		ArrayList<Integer> ascendingArr = ArrayListSorter.generateAscending(25);
		ArrayListSorter.quicksort(ascendingArr);
		assertEquals(25, ascendingArr.size());
		for(int i = 0; i < 25; ++i) {
			assertEquals(i + 1, ascendingArr.get(i));
		}
	}
	
	@Test
	void testQuickSortDuplicateList() {
		ArrayList<Integer> arr = new ArrayList<Integer>();
		arr.add(3);
		arr.add(3);
		arr.add(1);
		arr.add(6);
		arr.add(1);
		ArrayListSorter.quicksort(arr);
		assertEquals(5, arr.size());
		assertEquals(1, arr.get(0));
		assertEquals(1, arr.get(1));
		assertEquals(3, arr.get(2));
		assertEquals(3, arr.get(3));
		assertEquals(6, arr.get(4));
	}

	//GENERATE ASCENDING TESTS
	@Test
	void testGenerateAscending() {
		ArrayList<Integer> actual = ArrayListSorter.generateAscending(4);
		assertEquals(4, actual.size());
		assertEquals(1, actual.get(0));
		assertEquals(2, actual.get(1));
		assertEquals(3, actual.get(2));
		assertEquals(4, actual.get(3));
	}
	
	@Test
	void testGenerateAscendingSingle() {
		ArrayList<Integer> actual = ArrayListSorter.generateAscending(1);
		assertEquals(1, actual.size());
		assertEquals(1, actual.get(0));
	}
	
	@Test
	void testGenerateAscendingNegativeSize() {
		assertThrows(IllegalArgumentException.class, () -> {ArrayListSorter.generateAscending(-4);});
	}
	
	@Test
	void testGenerateAscendingLarge() {
		ArrayList<Integer> actual = ArrayListSorter.generateAscending(50);
		assertEquals(50, actual.size());
		for(int i = 0; i < 50; ++i)
		{
			assertEquals(i + 1, actual.get(i));
		}
	}

	//GENERATE PERMUTED TESTS
	@Test
	void testGeneratePermuted() {
		int size = 5;
		ArrayList<Integer> actual = ArrayListSorter.generatePermuted(size);
		assertEquals(5, actual.size());
		for(int i = 0; i < actual.size(); ++i)
		{
			assertTrue(actual.get(i) >= 1);
			assertTrue(actual.get(i) <= size);
		}
	}
	
	@Test
	void testGeneratePermutedLarge() {
		int size = 50;
		ArrayList<Integer> actual = ArrayListSorter.generateAscending(size);
		assertEquals(50, actual.size());
		for(int i = 0; i < size; ++i)
		{
			
		}
	}

	//GENERATE DESCENDING TESTS
	@Test
	void testGenerateDescending() {
		ArrayList<Integer> actual = ArrayListSorter.generateDescending(5);
		assertEquals(5, actual.size());
		assertEquals(5, actual.get(0));
		assertEquals(4, actual.get(1));
		assertEquals(3, actual.get(2));
		assertEquals(2, actual.get(3));
		assertEquals(1, actual.get(4));
	}
	
	@Test
	void testGenerateDescendingSingle() {
		ArrayList<Integer> actual = ArrayListSorter.generateDescending(1);
		assertEquals(1, actual.size());
		assertEquals(1, actual.get(0));
	}
	
	@Test
	void testGenerateDescendingEmpty() {
		ArrayList<Integer> actual = ArrayListSorter.generateDescending(0);
		assertEquals(0, actual.size());
	}
	
	@Test
	void testGenerateDescendingNegativeSize() {
		assertThrows(IllegalArgumentException.class, () -> {ArrayListSorter.generateDescending(-5);});
	}
	
	@Test
	void testGenerateDescendingLarge() {
		ArrayList<Integer> actual = ArrayListSorter.generateDescending(50);
		int k = 0;
		for(int i = 50; i > 0; --i)
		{
			assertEquals(i, actual.get(k));
			++k;
		}
	}

}
