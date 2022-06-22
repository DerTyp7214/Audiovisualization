package de.dertyp7214.audiovisualization.components

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Context
import android.content.ContextWrapper
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View
import androidx.fragment.app.FragmentActivity
import androidx.lifecycle.MutableLiveData
import de.dertyp7214.audiovisualization.R
import de.dertyp7214.audiovisualization.components.AudioVisualizationC.changeShortArraySize
import kotlin.random.Random

class AudioVisualizerView(context: Context, attrs: AttributeSet?, defStyle: Int) :
    View(context, attrs) {

    constructor(context: Context, attrs: AttributeSet?) : this(context, attrs, 0)

    private val mutableAudioData = MutableLiveData<ShortArray>()
    private val mutableBottomLeftCorner = MutableLiveData(Corner(0))
    private val mutableBottomRightCorner = MutableLiveData(Corner(0))
    private val mutableColor = MutableLiveData(Color.WHITE)

    private val audioData
        get() = mutableAudioData.value ?: shortArrayOf()
    private val bottomLeftCorner
        get() = mutableBottomLeftCorner.value ?: Corner(0)
    private val bottomRightCorner
        get() = mutableBottomRightCorner.value ?: Corner(0)
    private val color
        get() = mutableColor.value ?: Color.WHITE
    private val paint = Paint()
    private var bitmap: Bitmap? = null

    var size: Int = 64
    var visualize: Boolean = true

    init {
        val typedArray =
            context.obtainStyledAttributes(attrs, R.styleable.AudioVisualizerView, defStyle, 0)

        size = typedArray.getInt(R.styleable.AudioVisualizerView_size, size)
        if (typedArray.getBoolean(R.styleable.AudioVisualizerView_testInput, false))
            mutableAudioData.value = ShortArray(size) { Random.nextInt(255).toShort() }

        typedArray.recycle()
    }

    fun setColor(color: Int) = this.mutableColor.postValue(color)
    fun setAudioData(audioData: ShortArray, mirrored: Boolean = false) =
        this.mutableAudioData.postValue(audioData.let {
            if (mirrored) {
                val data =
                    it.changeShortArraySize(size / 2)
                data + data.reversed()
            } else it.changeShortArraySize(size)
        })

    fun setBottomLeftCorner(corner: Corner) = this.mutableBottomLeftCorner.postValue(corner)
    fun setBottomRightCorner(corner: Corner) = this.mutableBottomRightCorner.postValue(corner)

    @SuppressLint("DrawAllocation")
    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)

        viewTreeObserver.addOnGlobalLayoutListener {
            context.getActivity()?.let { activity ->
                if (activity is FragmentActivity) {
                    bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
                    mutableAudioData.observe(activity) {
                        invalidate()
                    }

                    mutableColor.observe(activity) {
                        paint.color = it
                        invalidate()
                    }
                }
            }
        }
    }

    @SuppressLint("DrawAllocation")
    override fun onDraw(canvas: Canvas) {
        drawOnBitmap(audioData, canvas)
    }

    private fun drawOnBitmap(audioData: ShortArray, canvas: Canvas) {
        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()
        val space = (width / size) / 10
        val barWidth = width / size - space

        canvas.drawRect(0f, 0f, 0f, 0f, Paint().apply {
            color = Color.TRANSPARENT
        })
        if (!visualize) return

        var i = 0
        var x = space / 2
        while (i < audioData.size) {
            val barHeight = height / 256f * (audioData[i] * .2f)

            val cornerBottomSpace = AudioVisualizationC.calculateBottomSpace(
                x,
                width,
                bottomLeftCorner.radius,
                bottomRightCorner.radius,
                barWidth,
                barHeight
            )

            canvas.drawRect(
                x,
                height - barHeight - cornerBottomSpace,
                width - (width - x) + barWidth,
                height,
                paint
            )

            i++
            x += barWidth + space
        }
    }

    private tailrec fun Context?.getActivity(): Activity? {
        return if (this == null) null
        else if (this !is ContextWrapper) null
        else if (this is Activity) this
        else baseContext.getActivity()
    }

    private fun <T> filledList(entry: T, amount: Int): List<T> {
        val tmp = ArrayList<T>()
        for (i in 0 until amount) tmp.add(entry)
        return tmp
    }

    class AudioStream(initialData: List<Short> = listOf()) {
        private val audioData = MutableLiveData(initialData)

        fun setNewAudioData(data: List<Short>) = audioData.postValue(data)

        fun observeAudioData(activity: FragmentActivity, callback: (List<Short>) -> Unit) =
            audioData.observe(activity, callback)
    }

    data class Corner(val radius: Int)
}