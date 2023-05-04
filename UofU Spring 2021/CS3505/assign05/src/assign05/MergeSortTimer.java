package assign05;

import java.util.ArrayList;

public class MergeSortTimer {

	public static void main(String[] args) {
		int timesToLoop = 90;
		
		int incr = 10_000;
		
		for(int probSize = 10_000; probSize <= 100_000; probSize += incr) 
		{
			//BEST CASE GOES HERE
			
			
			long stopTime, startTime = System.nanoTime(), totalTime = 0;
			
			while(System.nanoTime() - startTime < 1000000000) { // empty block
			}
			
			for(int i = 0; i < timesToLoop; i++) {
				ArrayList<Integer> arr = ArrayListSorter.generatePermuted(probSize);
				startTime = System.nanoTime();
				ArrayListSorter.mergesort(arr);
				stopTime = System.nanoTime();
				totalTime += stopTime - startTime;
				
			}
			
			double averageTime = totalTime / (double) timesToLoop;
			System.out.println(probSize + "  " + averageTime);
		}

	}

}
