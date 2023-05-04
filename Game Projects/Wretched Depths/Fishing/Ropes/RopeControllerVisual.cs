using System;
using System.Collections.Generic;
using UnityEngine;

public class RopeControllerVisual : MonoBehaviour
{

    private void OnDrawGizmos()
    {
        //draw dots at each segment position
        if (segments != null)
        {
            float colorFraction = 1f / (float) segments.Count+0.0001f;
            for (int i=0; i<segments.Count; i++)
            {
                GameObject segment = segments[i];
                Gizmos.color = new Color(1-colorFraction*i, 0, 0+colorFraction*i, 0.25f);
                Gizmos.DrawSphere(segment.transform.position, 0.1f);
            }
        }
        Gizmos.color = new Color(1, 0, 0, 0.25f);
        //draw line from start to end
        if (start && end)
        {
            Gizmos.DrawLine(start.transform.position, end.transform.position);
            Gizmos.DrawCube(start.transform.position, Vector3.one * 0.05f);
            Gizmos.DrawCube(end.transform.position, Vector3.one * 0.05f);
        }
    }

    [Header("References")]
    public GameObject start; //object at the start of the rope
    public GameObject end; //object at the end of the rope
    public LineRenderer lineRenderer; // line renderer for the rope

    [Header("Rope Settings")]
    [SerializeField]
    private GameObject segmentPrefab; // prefab for one segment of the rope 
    [SerializeField] 
    private int initialNumberOfSegments = 5; //number of segments that the rope is initialized to have
    [Range(0, 1.5f)] 
    public float slack = 1.1f; //slack of the rope

    // --Internal--
    private float length = 1f;  //current length of the rope
    private List<GameObject> segments;
    private int splineLength = 0;
    private float segmentLengthPrev = 0f;
    private GameObject newEnd = null;
    private Action OnEndUpdated = delegate { };

    //rendering
    private float[] xPositions;
    private float[] yPositions;
    private float[] zPositions;
    private CatmullRomSpline splineX;
    private CatmullRomSpline splineY;
    private CatmullRomSpline splineZ;
    private int splineFactor = 4;

    void Awake()
    {
        segments = new List<GameObject>();
        //bind the start of the rope to the start object
        SetSegmentCount(initialNumberOfSegments);
    }

    private void OnEnable()
    {
        RecalculateSplines();
    }

    private void Update()
    {
        //recalculate rope length
        AdjustRopeLengthToEndpoints();

        //update the length of the rope segments
        UpdateRopeSegmentLength();

        //this is very bad and stupid but it works 
        FixSegments();
    }

    void LateUpdate()
    {
        //------------DRAW THE ROPE-------------------------
        //get the values for positions for all segments
        if (segments != null)
        {
            if(segments.Count > 2)
            {
                for (var i = 0; i < segments.Count; i++)
                {
                    var position = segments[i].transform.position;
                    xPositions[i] = position.x;
                    yPositions[i] = position.y;
                    zPositions[i] = position.z;
                }

                xPositions[0] = start.transform.position.x;
                yPositions[0] = start.transform.position.y;
                zPositions[0] = start.transform.position.z;

                xPositions[segments.Count - 1] = end.transform.position.x;
                yPositions[segments.Count - 1] = end.transform.position.y;
                zPositions[segments.Count - 1] = end.transform.position.z;

                //use splines to calculate position of final vertex interpolating curve
                for (var i = 0; i < splineLength; i++)
                {
                    //draw line for segments
                    lineRenderer.SetPosition(i, new Vector3(
                        splineX.GetValue(i / (float)splineFactor),
                        splineY.GetValue(i / (float)splineFactor),
                        splineZ.GetValue(i / (float)splineFactor)));
                }
            }
            else
            {
                for (var i = 0; i < splineLength; i++)
                {
                    //draw line for segments
                    lineRenderer.SetPosition(i, Vector3.Lerp(start.transform.position, end.transform.position, (float) i/(float)(splineLength-1)));
                }
            }
        }
    }

    private void FixedUpdate()
    {
        //
        if (newEnd != null)
        {
            UpdateRopeEnd(newEnd);
        }
    }

    //------------------------ FUNCTIONS-------------------------------------------

    public void RecalculateSplines()
    {
        xPositions = new float[segments.Count];
        yPositions = new float[segments.Count];
        zPositions = new float[segments.Count];
        splineX = new CatmullRomSpline(xPositions);
        splineY = new CatmullRomSpline(yPositions);
        splineZ = new CatmullRomSpline(zPositions);
    }

    public Vector3 GetRopeDirection()
    {
        Vector3 d = segments[segments.Count - 2].transform.position - segments[segments.Count - 1].transform.position;
        return d;
    }

    public void SetRopeEnd(GameObject _end) => SetRopeEnd(_end, delegate { });

    public void SetRopeEnd(GameObject _end, Action _OnEndUpdated)
    {
        newEnd = _end;
        OnEndUpdated = _OnEndUpdated;
    }

    private void UpdateRopeEnd(GameObject _end)
    {

        //update end object
        end = _end;

        //destroy previous last segment
        GameObject segment = segments[segments.Count - 1];
        segment.transform.position = end.transform.position;

        //attach to end
        SetAttached(segment, end);

        OnEndUpdated();

        //reset vals
        newEnd = null;
        OnEndUpdated = delegate { };

        FixSegments();
    }

    public void RespawnSegments()
    {
        SetSegmentCount(segments.Count);
    }

    public void FixSegments()
    {
        foreach(GameObject segment in segments)
        {
            SpringJoint joint = segment.GetComponent<SpringJoint>();
            joint.autoConfigureConnectedAnchor = false;
            joint.anchor = Vector3.zero;
            joint.connectedAnchor = Vector3.zero;
        }
    }

    public void SetSegmentCount(int count)
    {
        //delete any existing segments
        for (int i = 0; i < segments.Count; i++)
        {
            Destroy(segments[i]);
        }

        //set number of segments and initialize storage
        segments = new List<GameObject>();

        //for every segment we need to create
        for (var i = 0; i < count; i++)
        {
            //spawn new segment at calculated position
            Vector3 pos = start.transform.position + ((end.transform.position - start.transform.position) * ((float)i / (count - 1)));
            GameObject segment = CreateSegment(pos,i);
            segments.Add(segment);

            //get refs to rigidbody and joint
            Rigidbody segmentBody = segment.GetComponent<Rigidbody>();
            SpringJoint segmentJoint = segment.GetComponent<SpringJoint>();

            //set the length of the segment
            AdjustRopeLengthToEndpoints();
            segmentJoint.minDistance = segmentJoint.maxDistance = segmentLengthPrev = length / (float)count;

            //NOT FIRST SEGMENT
            if (i > 0)
            {
                //set segment connected to previous
                segmentJoint.connectedBody = segments[i - 1].GetComponent<Rigidbody>();
            }
            //FIRST SEGMENT ONLY
            else { SetAttached(segment, start); }
            //LAST SEGMENT ONLY
            if (i == count - 1) { SetAttached(segment, end); }

            //do not auto-configure the anchor
            segmentJoint.autoConfigureConnectedAnchor = false;
        }

        //set up subdivisions for line renderer
        lineRenderer.positionCount = splineLength = ((count - 1) * splineFactor + 1);


        //initialize the positions and splines
        RecalculateSplines();
    }

    private GameObject CreateSegment(Vector3 pos, int i)
    {
        //set initial position to start object
        GameObject segment = Instantiate(segmentPrefab, pos, Quaternion.identity);
        segment.name = i + " " + start.name + "-" + end.name;
        segment.transform.SetParent(transform); //child of scripts GO
        return segment;
    }

    private void SetAttached(GameObject segment, GameObject attach)
    {
        //get rigidbody
        Rigidbody segmentBody = segment.GetComponent<Rigidbody>();

        //set parent
        segment.transform.SetParent(attach.transform);
        segmentBody.constraints = RigidbodyConstraints.FreezePosition;
        segmentBody.interpolation = RigidbodyInterpolation.None;
        segmentBody.isKinematic = true;
    }

    private void AdjustRopeLengthToEndpoints()
    {
        //set the rope length to the distance between startObject and endObject * slack
        //Debug.DrawLine(startObject.transform.position, endObject.transform.position, Color.blue, 0.05f);
        float newRopeLength = Vector3.Distance(start.transform.position, end.transform.position) * slack;
        length = newRopeLength;
    }

    private void UpdateRopeSegmentLength()
    {
        //calculate and the segment length
        float segmentLength = length / (float)(segments.Count - 1);

        //Debug.Log(segmentLength.ToString("0.000") + "  " + ((segments.Count - 1) * segmentLength).ToString("0.000") + " " + length.ToString("0.000"));

        //if the segment length is not equal to the previous segmentLength
        if (segmentLengthPrev != segmentLength)
        {
            //replace segment length for all joints
            for (int i = 0; i < segments.Count; i++)
            {
                SpringJoint joint = segments[i].GetComponent<SpringJoint>();
                joint.maxDistance = joint.minDistance = segmentLength;
            }

            //agitate the rope so it is forced to update length
            if (segments[1])
            {
                segments[1].GetComponent<Rigidbody>().AddForce(new Vector3(0.01f, 0.01f, 0.01f));
            }
        }

        //Debug.LogError((int)length + "  " + (int)Vector3.Distance(start.transform.position, end.transform.position) + "   " + segments.Count + "      " + segments[1].GetComponent<Rigidbody>().drag + "    " + segments[1].GetComponent<SpringJoint>().maxDistance + "    " + segmentLength);

        //update previous segment length
        segmentLengthPrev = segmentLength;
    }

    private void OnDestroy()
    {
        for (int i = 0; i < segments.Count; i++)
        {
            Destroy(segments[i]);
        }
    }
}

