package com.example.androidndksample

import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class FeaturesAdapter(
    private var items: List<String>
) : RecyclerView.Adapter<FeaturesAdapter.FeatureViewHolder>() {

    class FeatureViewHolder(val tv: TextView) : RecyclerView.ViewHolder(tv)

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): FeatureViewHolder {
        val tv = LayoutInflater.from(parent.context)
            .inflate(android.R.layout.simple_list_item_1, parent, false) as TextView
        return FeatureViewHolder(tv)
    }

    override fun onBindViewHolder(holder: FeatureViewHolder, position: Int) {
        holder.tv.text = items[position]
    }

    override fun getItemCount() = items.size

    fun updateData(newItems: List<String>) {
        items = newItems
        notifyDataSetChanged()
    }
}