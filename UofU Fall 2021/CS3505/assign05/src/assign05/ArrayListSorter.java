package assign05;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class ArrayListSorter<T> {

	private static int thresh = 3;

	/**
	 * Sorts an object array from lowest to highest using mergesort
	 * 
	 * @param <T> an object type
	 * @param arr an array of T objects
	 */
	public static <T extends Comparable<? super T>> void mergesort(ArrayList<T> arr) {
		int rightIndex = arr.size() - 1;
		int leftIndex = 0;
		mergesort(arr, leftIndex, rightIndex);
	}

	/**
	 * Sorts an object array from a specified left index to a specified right index
	 * 
	 * @param <T>   an object type
	 * @param arr   an Array of T objects
	 * @param left  index of the beginning of a subarray
	 * @param right index of the end of a subarray
	 */
	public static <T extends Comparable<? super T>> void mergesort(ArrayList<T> arr, int left, int right) {

		if ((right - left) == thresh) {
			int mid = (left + right) / 2;
			insertionSort(arr, left, right);
			merge(arr, left, mid + 1, right);
		} else if (left < right) {
			int mid = (left + right) / 2;
			mergesort(arr, left, mid);
			mergesort(arr, mid + 1, right);
			merge(arr, left, mid + 1, right);
		}

	}

	private static <T extends Comparable<? super T>> void insertionSort(ArrayList<T> objArr, int start, int end) {
		for (int i = start + 1; i <= end; ++i) {
			T obj = objArr.get(i);
			int j;
			for (j = i - 1; j >= start && ((objArr.get(j)).compareTo(obj)) > 0; --j) {
				objArr.set((j + 1), objArr.get(j));
			}
			objArr.set(j + 1, obj);
		}
	}

	private static <T extends Comparable<? super T>> void merge(ArrayList<T> arr, int start, int mid, int end) {
		ArrayList<T> tempArr = new ArrayList<T>((end - start) + 1);
		int leftCursor = start;
		int rightCursor = mid;
		while (leftCursor < mid && rightCursor <= end) {
			if (arr.get(leftCursor).compareTo(arr.get(rightCursor)) < 0) {
				tempArr.add(arr.get(leftCursor));
				++leftCursor;
			} else {
				tempArr.add(arr.get(rightCursor));
				++rightCursor;
			}
		}

		if (leftCursor < mid) {
			while (leftCursor < mid) {
				tempArr.add(arr.get(leftCursor));
				++leftCursor;
			}
		}

		if (rightCursor <= end) {
			while (rightCursor <= end) {
				tempArr.add(arr.get(rightCursor));
				++rightCursor;
			}
		}

		int tempInd = 0;
		for (int i = start; i <= end; ++i) {
			arr.set(i, tempArr.get(tempInd));
			++tempInd;
		}
	}

	public static <T extends Comparable<? super T>> void quicksort(ArrayList<T> arr) {
		int left = 0;
		int right = arr.size() - 1;
		quicksort(arr, left, right);
	}

	public static <T extends Comparable<? super T>> void quicksort(ArrayList<T> arr, int left, int right) {
		if (left < right) {
			if (right - left < 3) 
			{
				insertionSort(arr, left, right);
			} 
			else 
			{
				int pivotPoint = partition(arr, left, right);
				quicksort(arr, left, pivotPoint - 1);
				quicksort(arr, pivotPoint + 1, right);
			}
		}

	}

	public static <T extends Comparable<? super T>> int partition(ArrayList<T> arr, int left, int right) {
		int pivot = getPivot(arr, left, right);
		T obj = arr.get(right);
		arr.set(right, arr.get(pivot));
		arr.set(pivot, obj);
		int leftCursor = left;
		int rightCursor = right - 1;
		while (leftCursor < rightCursor) {
			while (arr.get(leftCursor).compareTo(arr.get(right)) <= 0) {
				++leftCursor;
			}
			while (arr.get(rightCursor).compareTo(arr.get(right)) > 0) {
				--rightCursor;
			}
			if (leftCursor < rightCursor) {
				obj = arr.get(leftCursor);
				arr.set(leftCursor, arr.get(rightCursor));
				arr.set(rightCursor, obj);
			}
		}
		obj = arr.get(leftCursor);
		arr.set(leftCursor, arr.get(right));
		arr.set(right, obj);

		return rightCursor + 1;
	}

	private static <T extends Comparable<? super T>> int getPivot(ArrayList<T> arr, int left, int right) {
		int mid = (right - left) / 2;
		if (arr.get(left).compareTo(arr.get(right)) < 0) {
			if (arr.get(left).compareTo(arr.get(mid)) > 0) {
				return left;
			} else if (arr.get(mid).compareTo(arr.get(right)) > 0) {
				return right;
			} else {
				return mid;
			}
		} else if (arr.get(right).compareTo(arr.get(left)) < 0) {
			if (arr.get(right).compareTo(arr.get(mid)) > 0) {
				return right;
			} else if (arr.get(mid).compareTo(arr.get(left)) > 0) {
				return left;
			} else {
				return mid;
			}
		} else if (arr.get(left).compareTo(arr.get(mid)) == 0 || arr.get(right).compareTo(arr.get(mid)) == 0) {
			return mid;
		}
		return right;
	}

	/**
	 * Returns an ArrayList<Integer> counting from 1 to the indicated size in
	 * ascending order.
	 * 
	 * @param size the size of the array
	 * @return array sorted from 1 to size in ascending order
	 */
	public static ArrayList<Integer> generateAscending(int size) {
		if (size < 0) {
			throw new IllegalArgumentException();
		}
		ArrayList<Integer> sortedArr = new ArrayList<Integer>();
		for (int i = 1; i <= size; ++i) {
			sortedArr.add(i);
		}
		return sortedArr;
	}

	/**
	 * Returns an ArrayList<Integer> shuffled in a random order, with a range of 1
	 * to the size
	 * 
	 * @param size the size of the array
	 * @return a shuffled ArrayList ranging from 1 to size
	 */
	public static ArrayList<Integer> generatePermuted(int size) {
		if (size < 0) {
			throw new IllegalArgumentException();
		}
		Random rand = new Random(10);
		ArrayList<Integer> sortedArr = generateAscending(size);
		Collections.shuffle(sortedArr, rand);
		return sortedArr;
	}

	/**
	 * Returns an ArrayList<Integer> counting from the desired size down to 1 in
	 * descending order
	 * 
	 * @param size the size of the array
	 * @return An ArrayList<Integer> in descending order
	 */
	public static ArrayList<Integer> generateDescending(int size) {
		if (size < 0) {
			throw new IllegalArgumentException();
		}
		ArrayList<Integer> sortedArr = new ArrayList<Integer>();
		for (int i = size; i >= 1; --i) {
			sortedArr.add(i);
		}
		return sortedArr;
	}

}
