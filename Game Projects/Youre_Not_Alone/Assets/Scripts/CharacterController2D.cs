using System.Collections;
using UnityEngine;
using UnityEngine.Events;

public class CharacterController2D : MonoBehaviour
{
	[SerializeField] private float m_JumpForce = 400f;                          // Amount of force added when the player jumps.
	[Range(0, .3f)][SerializeField] private float m_MovementSmoothing = .05f;   // How much to smooth out the movement
	[SerializeField] private bool m_AirControl = false;                         // Whether or not a player can steer while jumping;
	[SerializeField] private LayerMask m_WhatIsGround;                          // A mask determining what is ground to the character
	[SerializeField] private Transform m_GroundCheck;                           // A position marking where to check if the player is grounded.

	const float k_GroundedRadius = .2f; // Radius of the overlap circle to determine if grounded
	private TrailRenderer _trailRenderer;
	private bool m_Grounded;            // Whether or not the player is grounded.
	private Rigidbody2D m_Rigidbody2D;
	private bool m_FacingRight = true;  // For determining which way the player is currently facing.
	private Vector3 m_Velocity = Vector3.zero;

	private bool canDoubleJump;
	private bool hasDoubleJumped = false;

	public Animator animator;
	public AudioManager audioPlayer;

	[Header("Toggles")]
	public static bool doubleJumpEnabled = false;
	public static bool dashEnabled = false;

	[Header("Dashing")]
	[SerializeField] private float _dashingVelocity = 14f;
	[SerializeField] private float _dashingTime = 0.5f;
	[SerializeField] private float _dashingFloatiness = 7.5f;
	private Vector2 _dashingDir;
	private bool _isDashing;
	private bool _canDash = true;
	private Vector2 oldVelocity;


	[Header("Events")]
	[Space]

	public UnityEvent OnLandEvent;

	[System.Serializable]
	public class BoolEvent : UnityEvent<bool> { }

	private void Awake()
	{
		canDoubleJump = true;
		m_Rigidbody2D = GetComponent<Rigidbody2D>();
		_trailRenderer = GetComponent<TrailRenderer>();
		//audioPlayer = AudioManager.instance;

		if (OnLandEvent == null)
			OnLandEvent = new UnityEvent();
	}

    private void Start()
    {
		audioPlayer.PlaySound("BGM");
		audioPlayer.PlaySound("Wind");
	}

    private void FixedUpdate()
	{
		bool wasGrounded = m_Grounded;
		m_Grounded = false;

		// The player is grounded if a circlecast to the groundcheck position hits anything designated as ground
		// This can be done using layers instead but Sample Assets will not overwrite your project settings.
		Collider2D[] colliders = Physics2D.OverlapCircleAll(m_GroundCheck.position, k_GroundedRadius, m_WhatIsGround);
		for (int i = 0; i < colliders.Length; i++)
		{
			if (colliders[i].gameObject != gameObject)
			{
				m_Grounded = true;
				if (!wasGrounded)
					OnLandEvent.Invoke();
				animator.SetBool("IsJumping", false);
				canDoubleJump = true;
			}
		}

	}


	public void Move(float move, float vertical, bool jump, bool dash, float isMoving)
	{
		
		if (m_Grounded && isMoving != 0)    // Player is moving
		{
			audioPlayer.PlaySound("Walking (Grass)");
		}
		//only control the player if grounded or airControl is turned on
		if (m_Grounded || m_AirControl)
		{
			// Move the character by finding the target velocity
			Vector3 targetVelocity = new Vector2(move * 10f, m_Rigidbody2D.velocity.y);
			// And then smoothing it out and applying it to the character
			m_Rigidbody2D.velocity = Vector3.SmoothDamp(m_Rigidbody2D.velocity, targetVelocity, ref m_Velocity, m_MovementSmoothing);

			// If the input is moving the player right and the player is facing left...
			if (move > 0 && !m_FacingRight)
			{
				// ... flip the player.
				Flip();
			}
			// Otherwise if the input is moving the player left and the player is facing right...
			else if (move < 0 && m_FacingRight)
			{
				// ... flip the player.
				Flip();
			}
		}

		animator.SetFloat("Speed", Mathf.Abs(isMoving)); // ilde and walk animation

		// If the player should jump...
		if (m_Grounded && jump)
		{
			audioPlayer.PlaySound("Jump");


			m_Grounded = false;

			// Add a vertical force to the player.
			m_Rigidbody2D.velocity = new Vector2(m_Rigidbody2D.velocity.x, m_JumpForce / 50);
			StartCoroutine(StartJumpAnimation());
		}
		else if (jump && canDoubleJump && doubleJumpEnabled)
		{
			audioPlayer.PlaySound("Jump");

			canDoubleJump = false;
			hasDoubleJumped = true;

			// Add a vertical force to the player.
			m_Rigidbody2D.velocity = new Vector2(m_Rigidbody2D.velocity.x, m_JumpForce / 50);
			StartCoroutine(StartJumpAnimation());
		}

		if (dash && _canDash && dashEnabled)
		{
			dash = false;
			audioPlayer.PlaySound("Dash");

			oldVelocity = m_Rigidbody2D.velocity;
			_isDashing = true;
			_canDash = false;
			_trailRenderer.emitting = true;
			//_dashingDir = new Vector2(move, Input.GetAxisRaw("Vertical"));
			_dashingDir = new Vector2(move, vertical);
			if (_dashingDir == Vector2.zero)
			{
				_dashingDir = new Vector2(transform.localScale.x, 0);
			}
			StartCoroutine(StopDashing(_dashingDir));
		}
		if (_isDashing)
		{
			canDoubleJump = false;
			m_Rigidbody2D.velocity = _dashingDir.normalized * _dashingVelocity;
			StartCoroutine(StartDashAnimation());
		}
		if (m_Grounded && !_isDashing)
		{
			_canDash = true;
			hasDoubleJumped = false;
			StartCoroutine(StopDashAnimation());
		}
	}

	private IEnumerator StopDashing(Vector2 dashingDir)
	{
		yield return new WaitForSeconds(_dashingTime);
		dashingDir.Normalize();
		//This block of code adds velocity to the character after a dash to gove them some hang time in the air.
		if (dashingDir.y > 0)
		{
			if (dashingDir.x != 0)
			{
				m_Rigidbody2D.velocity = new Vector2(oldVelocity.x, _dashingFloatiness);
			}
			else
			{
				m_Rigidbody2D.velocity = new Vector2(0, _dashingFloatiness);
			}
		}
		else if (dashingDir.y == 0)
		{
			m_Rigidbody2D.velocity = new Vector2(oldVelocity.x + _dashingFloatiness, 0);
		}

		if (!hasDoubleJumped)
		{
			canDoubleJump = true;
		}
		_trailRenderer.emitting = false;
		_isDashing = false;
	}

	public void Flip()
	{
		// Switch the way the player is labelled as facing.
		m_FacingRight = !m_FacingRight;

		// Multiply the player's x local scale by -1.
		Vector3 theScale = transform.localScale;
		theScale.x *= -1;
		transform.localScale = theScale;
	}

	public void flipIfFacingLeft()
    {
		if (!m_FacingRight)
        {
			Flip();
        }
	}

	private IEnumerator StartJumpAnimation()
	{
		yield return new WaitForSeconds(0.01f);

		animator.SetBool("IsJumping", true);
	}

	private IEnumerator StartDashAnimation()
	{
		yield return new WaitForSeconds(0.01f);

		animator.SetBool("IsDashing", true);
	}
	private IEnumerator StopDashAnimation()
	{
		yield return new WaitForSeconds(0.01f);

		animator.SetBool("IsDashing", false);
	}

	public void EnableDoubleJump()
    {
		doubleJumpEnabled = true;
    }

	public void EnableDash()
	{
		dashEnabled = true;
	}
}